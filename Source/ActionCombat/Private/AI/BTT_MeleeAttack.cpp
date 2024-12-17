// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_MeleeAttack.h"

#include "AIController.h"
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

void UBTT_MeleeAttack::MoveTowardsPlayer()
{
	ACharacter* TargetCharacter{GetWorld()->GetFirstPlayerController()->GetCharacter()};
	if (!IsValid(TargetCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("TargetCharacter is nullptr!"));
		return;
	}
	FAIMoveRequest MoveRequest{TargetCharacter};
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAcceptanceRadius(MoveAcceptanceRadius);

	AIController->MoveTo(MoveRequest);
	AIController->SetFocus(TargetCharacter);

	AIController->ReceiveMoveCompleted.AddUniqueDynamic(this, &UBTT_MeleeAttack::HandleMoveCompleted);
	UE_LOG(LogTemp, Warning, TEXT("Moving to player"));
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bIsFinished = false;
	AIController = OwnerComp.GetAIOwner();
	float DistanceToPlayer{OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceToPlayer"))};
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("MeleeAttackRadius"), AttackRadius);

	
	IFighter* FighterInterfacePtr{Cast<IFighter>(AIController->GetCharacter())};
	if (!FighterInterfacePtr)
	{
		UE_LOG(LogTemp, Error, TEXT("The AI character does not implement the IFighter interface!"));
		return EBTNodeResult::Failed;
	}
	
	if (DistanceToPlayer > AttackRadius)
	{
		// First play the pre-attack animation
		FighterInterfacePtr->MeleeAttack();
		float PreAttackAnimDuration{FighterInterfacePtr->GetAnimDuration()};
		FTimerHandle PreAttackTimerHandle;

		// Move towards the player after the pre-attack animation
		GetWorld()->GetTimerManager().SetTimer(PreAttackTimerHandle, this, &UBTT_MeleeAttack::MoveTowardsPlayer, PreAttackAnimDuration,
		                                       false);
		return EBTNodeResult::InProgress;
	}
	else if (DistanceToPlayer != 0.0f) // If the player is within the attack radius (DistanceToPlayer should never be 0)
	{
		FighterInterfacePtr->MeleeAttack();
		float AttackAnimDuration{FighterInterfacePtr->GetAnimDuration()};
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBTT_MeleeAttack::FinishAttackTask, AttackAnimDuration,
												   false);
		return EBTNodeResult::InProgress;
	}

	UE_LOG(LogTemp, Error, TEXT("Player's distance is %f"), DistanceToPlayer);
	return EBTNodeResult::Failed;
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	float DistanceToPlayer{OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceToPlayer"))};
	IFighter* FighterInterfacePtr{Cast<IFighter>(AIController->GetCharacter())};
	if (DistanceToPlayer > FighterInterfacePtr->GetMeleeRange())
	{
		AIController->StopMovement();
		AIController->ClearFocus(EAIFocusPriority::Gameplay);

		OwnerComp.GetAIOwner()->ReceiveMoveCompleted.RemoveDynamic(this, &UBTT_MeleeAttack::HandleMoveCompleted);

		OwnerComp.GetBlackboardComponent()->
		          SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(EEnemyState::Range));
		return FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
	}

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
