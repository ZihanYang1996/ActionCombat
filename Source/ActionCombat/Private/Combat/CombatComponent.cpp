// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatComponent.h"

#include "GameFramework/Character.h"
#include "Interfaces/MainPlayer.h"

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
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::CombatAttack()
{
	// Check if the player is attacking, no input will be accepted if the player is attacking
	if (bIsAttacking)
	{
		return;
	}

	// Check if we can continue the combo
	if (bCanContinueCombo)
	{
		CurrentAttackMontageIndex = (CurrentAttackMontageIndex + 1) % MaxAttackMontageIndex;
	}
	else
	{
		CurrentAttackMontageIndex = 0;
	}

	// Check if the player has enough stamina to perform the attack
	if (OwnerCharacter->Implements<UMainPlayer>() && IMainPlayer::Execute_HasEnoughStamina(
		OwnerCharacter, AttackStaminaCost))
	{
		// Set the attacking flag to true, to prevent the player from attacking again
		bIsAttacking = true;

		// If the player has enough stamina, reduce the stamina
		OnAttackPerformedDelegate.Broadcast(AttackStaminaCost);

		// Play the attack montage (Perform the attack)
		OwnerCharacter->PlayAnimMontage(AttackMontages[CurrentAttackMontageIndex]);
	}
}

void UCombatComponent::EnableComboContinuation()
{
	// Allow the player to continue the combo
	bIsAttacking = false;
	bCanContinueCombo = true;
}

void UCombatComponent::ResetCombo()
{
	// Reset the combo
	bCanContinueCombo = false;
	// CurrentAttackMontageIndex = 0;
}

void UCombatComponent::RandomAttack()
{
	int RandomIndex{FMath::RandRange(0, AttackMontages.Num() - 1)};
	AnimDuration = OwnerCharacter->PlayAnimMontage(AttackMontages[RandomIndex]);
}
