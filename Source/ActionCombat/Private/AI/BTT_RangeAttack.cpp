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
	
	FighterInterfacePtr->RangedAttack();

	// Generate a random number between 0 and 1
	const float RandomValue{FMath::FRand()};
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
		// Decrease the threshold
		CurrentChargeAttackTransitionThreshold -= 0.1f;
	}
	return EBTNodeResult::Succeeded;
}
