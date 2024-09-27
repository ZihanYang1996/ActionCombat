// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/LookAtPlayerComponent.h"

// Sets default values for this component's properties
ULookAtPlayerComponent::ULookAtPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULookAtPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPtr = GetOwner();
	// ...
	
}


// Called every frame
void ULookAtPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LookAtPlayer();
}

void ULookAtPlayerComponent::LookAtPlayer()
{
	FVector PlayerLocation{GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()};
	FRotator TargetRotation{FRotationMatrix::MakeFromX(PlayerLocation - OwnerPtr->GetActorLocation()).Rotator()};

	FRotator NewRotation{FMath::RInterpTo(OwnerPtr->GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed)};

	OwnerPtr->SetActorRotation(NewRotation);
	
}

