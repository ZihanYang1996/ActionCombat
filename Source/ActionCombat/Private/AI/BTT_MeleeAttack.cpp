// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_MeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/Fighter.h"
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

		OwnerComp.GetAIOwner()->ReceiveMoveCompleted.AddUniqueDynamic(this, &UBTT_MeleeAttack::HandleMoveCompleted);
		UE_LOG(LogTemp, Warning, TEXT("Moving to player"));
		return EBTNodeResult::InProgress;
	}
	else if (DistanceToPlayer != 0.0f) // If the player is within the attack radius (DistanceToPlayer should never be 0)
	{
		IFighter* FighterInterfacePtr{Cast<IFighter>(OwnerComp.GetAIOwner()->GetCharacter())};
		if (FighterInterfacePtr)
		{
			FighterInterfacePtr->Attack();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("The AI character does not implement the IFighter interface!"));
		}

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBTT_MeleeAttack::FinishAttackTask, 2.0f, false);

		return EBTNodeResult::InProgress;
	}

	UE_LOG(LogTemp, Error, TEXT("Player's distance is %f"), DistanceToPlayer);
	return EBTNodeResult::Failed;
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bIsFinished)
	{
		OwnerComp.GetAIOwner()->ReceiveMoveCompleted.RemoveDynamic(this, &UBTT_MeleeAttack::HandleMoveCompleted);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTT_MeleeAttack::HandleMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	FinishAttackTask();
	
}

void UBTT_MeleeAttack::FinishAttackTask()
{
	bIsFinished = true;
}