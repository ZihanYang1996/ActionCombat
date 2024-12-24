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

	// Initialize and set up the melee attack moves
	for (TSubclassOf<UAttackMove> AttackMove : MeleeAttackMoves)
	{
		UAttackMove* AttackInstance{NewObject<UAttackMove>(this, AttackMove)};
		AttackInstance->Setup(OwnerCharacter);
		MeleeAttackMoveInstances.Add(AttackInstance);
	}

	// Initialize and set up the ranged attack moves
	for (TSubclassOf<UAttackMove> AttackMove : RangedAttackMoves)
	{
		UAttackMove* AttackInstance{NewObject<UAttackMove>(this, AttackMove)};
		AttackInstance->Setup(OwnerCharacter);
		RangedAttackMoveInstances.Add(AttackInstance);
	}

	// Initialize and set up the charge attack moves
	for (TSubclassOf<UAttackMove> AttackMove : ChargeAttackMoves)
	{
		UAttackMove* AttackInstance{NewObject<UAttackMove>(this, AttackMove)};
		AttackInstance->Setup(OwnerCharacter);
		ChargeAttackMoveInstances.Add(AttackInstance);
	}

	
	
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
		bCanTakeInput = false;

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
	bCanTakeInput = true;  // Allow the player to take input (roll, for example)
	bCanContinueCombo = true;
}

void UCombatComponent::ResetCombo()
{
	// Reset the combo 
	bIsAttacking = false;  // Reset the attacking flag
	bCanTakeInput = true;  // Allow the player to take input
	bCanContinueCombo = false;
	// CurrentAttackMontageIndex = 0;
}


void UCombatComponent::ExecuteRandomAttack(const TArray<UAttackMove*>& AttackMoveInstances)
{
	int RandomIndex{FMath::RandRange(0, AttackMoveInstances.Num() - 1)};
	
	if (AttackMoveInstances.IsValidIndex(RandomIndex))
	{
		if (!IsValid(AttackMoveInstances[RandomIndex]))
		{
			UE_LOG(LogTemp, Error, TEXT("Attack Move Instance is null"));
			return;
		}
		if (!IsValid(OwnerCharacter))
		{
			UE_LOG(LogTemp, Error, TEXT("OwnerCharacter is null"));
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("%f"), AnimDuration);
		AnimDuration = AttackMoveInstances[RandomIndex]->Execute();
	}
}


void UCombatComponent::AIMeleeAttack()
{
	ExecuteRandomAttack(MeleeAttackMoveInstances);
}

void UCombatComponent::AIRangedAttack()
{
	ExecuteRandomAttack(RangedAttackMoveInstances);
}

void UCombatComponent::AIChargeAttack()
{
	ExecuteRandomAttack(ChargeAttackMoveInstances);
}



