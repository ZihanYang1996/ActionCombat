// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_RangeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"
#include "GameFramework/Character.h"
#include "Interfaces/Fighter.h"

EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef{OwnerComp.GetAIOwner()->GetCharacter()};
	bIsFinished = false;
	if (!IsValid(CharacterRef))
	{
		return EBTNodeResult::Failed;
	}

	float DistanceToPlayer{OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceToPlayer"))};
	IFighter* FighterInterfacePtr{Cast<IFighter>(CharacterRef)};
	if (!FighterInterfacePtr)
	{
		return EBTNodeResult::Failed;
	}
	if (DistanceToPlayer < FighterInterfacePtr->GetMeleeRange())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(
			TEXT("CurrentState"), static_cast<uint8>(EEnemyState::Melee));
		AbortTask(OwnerComp, NodeMemory);
		return EBTNodeResult::Aborted;
	}

	// Generate a random number between 0 and 1
	const float RandomValue{FMath::FRand()};
	UE_LOG(LogTemp, Warning, TEXT("Random Value: %f"), CurrentChargeAttackTransitionThreshold);
	if (RandomValue > CurrentChargeAttackTransitionThreshold)
	{
		// Reset the threshold
		CurrentChargeAttackTransitionThreshold = ChargeAttackTransitionThreshold;

		// Transition to the charge attack
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(
			TEXT("CurrentState"), static_cast<uint8>(EEnemyState::Charge));
	}	
	else
	{
		FighterInterfacePtr->RangedAttack();
		float AnimDuration{FighterInterfacePtr->GetAnimDuration()};
		FTimerHandle AttackTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UBTT_RangeAttack::FinishAttackTask, AnimDuration, false);
		// Decrease the threshold
		CurrentChargeAttackTransitionThreshold -= 0.4f;
	}
	return EBTNodeResult::InProgress;
}

void UBTT_RangeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bIsFinished)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_RangeAttack::FinishAttackTask()
{
	bIsFinished = true;
}

UBTT_RangeAttack::UBTT_RangeAttack()
{
	NodeName = TEXT("Range Attack");
	bNotifyTick = true; // This will make the TickTask function be called
	// bCreateNodeInstance = true;  // Since there is only one enemy, we don't need to create a new instance of this node
}
