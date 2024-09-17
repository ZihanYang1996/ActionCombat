// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatComponent.h"

#include "GameFramework/Character.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	MaxAttackMontageIndex = AttackMontages.Num();
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::CombatAttack()
{
	OwnerCharacter->PlayAnimMontage(AttackMontages[NextAttackMontageIndex]);

	// Increment the index and wrap around
	NextAttackMontageIndex = (NextAttackMontageIndex + 1) % MaxAttackMontageIndex;
	
}

