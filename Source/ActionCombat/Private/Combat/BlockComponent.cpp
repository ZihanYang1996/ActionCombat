// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BlockComponent.h"

#include "GameFramework/Character.h"
#include "Interfaces/MainPlayer.h"

// Sets default values for this component's properties
UBlockComponent::UBlockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBlockComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = GetOwner<ACharacter>();
	// ...
	
}


// Called every frame
void UBlockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UBlockComponent::CanBlock(AActor* DamageCauser) const
{
	// Check if the owner character implements the IMainPlayer interface
	if (!OwnerCharacter->Implements<UMainPlayer>())
	{
		return false;
	}

	// Check if the owner character has enough stamina to block
	if (!IMainPlayer::Execute_HasEnoughStamina(OwnerCharacter, StaminaCost))
	{
		return false;
	}

	// Check if the damage causer is in front of the player
	FVector DirectionToDamageCauser{(DamageCauser->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal()};
	FVector OwnerForward{OwnerCharacter->GetActorForwardVector()};

	double DotProduct{FVector::DotProduct(DirectionToDamageCauser, OwnerForward)};

	if (DotProduct < 0.0f)
	{
		// The damage causer is behind the player
		return false;
	}
	
	return true;
}

