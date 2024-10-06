// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_MeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_MeleeAttack::UBTT_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
	bNotifyTick = true;
	
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bIsFinished = false;
	float DistanceToPlayer{OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceToPlayer"))};

	if (DistanceToPlayer > AttackRadius)
	{
		ACharacter* TargetCharacter{GetWorld()->GetFirstPlayerController()->GetCharacter()};
		if (!IsValid(TargetCharacter))
		{
			UE_LOG(LogTemp, Error, TEXT("TargetCharacter is nullptr!"));
			return EBTNodeResult::Failed;
		}
		FAIMoveRequest MoveRequest{TargetCharacter};
		MoveRequest.SetUsePathfinding(true);
		MoveRequest.SetAcceptanceRadius(MoveAcceptanceRadius);

		OwnerComp.GetAIOwner()->MoveTo(MoveRequest);
		OwnerComp.GetAIOwner()->SetFocus(TargetCharacter);

		OwnerComp.GetAIOwner()->ReceiveMoveCompleted.AddUniqueDynamic(this, &UBTT_MeleeAttack::FinishAttackTask);
		UE_LOG(LogTemp, Warning, TEXT("Moving to player"));
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
	
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bIsFinished)
	{
		OwnerComp.GetAIOwner()->ReceiveMoveCompleted.RemoveDynamic(this, &UBTT_MeleeAttack::FinishAttackTask);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_MeleeAttack::FinishAttackTask(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	bIsFinished = true;
}