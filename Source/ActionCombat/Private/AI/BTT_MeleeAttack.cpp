// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_MeleeAttack.h"

#include "AIController.h"
#include "AttackMoves/AttackMove.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"
#include "GameFramework/Character.h"
#include "Interfaces/Fighter.h"
#include "Navigation/PathFollowingComponent.h"


UBTT_MeleeAttack::UBTT_MeleeAttack()
{
	NodeName = TEXT("Melee MeleeAttack");
	bNotifyTick = true;
}


EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = OwnerComp.GetAIOwner();
	// Reset the IsMeleeMoveTowardsPlayerComplete (bIsFinished) key to false
	AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsMeleeMoveTowardsPlayerComplete"), false);

	IFighter* FighterInterfacePtr{Cast<IFighter>(AIController->GetCharacter())};
	if (!FighterInterfacePtr)
	{
		UE_LOG(LogTemp, Error, TEXT("The AI character does not implement the IFighter interface!"));
		return EBTNodeResult::Failed;
	}

	FighterInterfacePtr->MeleeAttack();
	if (FighterInterfacePtr->GetAnimDuration() == 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Something went wrong with the Melee Attack!"));
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::InProgress;
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	float DistanceToPlayer{OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceToPlayer"))};
	IFighter* FighterInterfacePtr{Cast<IFighter>(AIController->GetCharacter())};
	bIsFinished = AIController->GetBlackboardComponent()->GetValueAsBool(TEXT("IsMeleeMoveTowardsPlayerComplete"));

	if (DistanceToPlayer > FighterInterfacePtr->GetMeleeRange())
	{
		AIController->StopMovement();
		AIController->ClearFocus(EAIFocusPriority::Gameplay);

		UAttackMove* AttackMove{
			Cast<UAttackMove>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("CurrentAttackMove")))
		};

		AttackMove->FinishAttackTask();
		OwnerComp.GetBlackboardComponent()->
		          SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(EEnemyState::Range));
		return FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
	}

	if (bIsFinished)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
