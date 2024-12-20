// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackMoves/Melee_LR.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/ContinueAttackNotify.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"

void UMelee_LR::FinishAttackTask()
{
	// UE_LOG(LogTemp, Warning, TEXT("Finishing the attack task"));
	AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsMeleeMoveTowardsPlayerComplete"), true);
	AIController->ReceiveMoveCompleted.RemoveDynamic(this, &UMelee_LR::HandleMoveCompleted);
	AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("CurrentAttackMove"), nullptr);
}

float UMelee_LR::Execute()
{
	if (!IsValid(AIController))
	{
		UE_LOG(LogTemp, Error, TEXT("AIController is nullptr!"));
		return 0.0f;
	}

	// Set this AttackMove as the current attack move
	AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("CurrentAttackMove"), this);

	float DistanceToPlayer{AIController->GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceToPlayer"))};
	if (DistanceToPlayer > AttackRadius)
	{
		PreAttackAnimDuration = Attacker->PlayAnimMontage(PreAttack);
		UE_LOG(LogTemp, Warning, TEXT("Distance to player is greater than attack radius"));
		FTimerHandle PreAttackTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(PreAttackTimerHandle, this, &UMelee_LR::MoveTowardsPlayer,
		                                       PreAttackAnimDuration,
		                                       false);
		return PreAttackAnimDuration;
	}
	else if (DistanceToPlayer != 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Distance to player is less than attack radius"));
		UE_LOG(LogTemp, Warning, TEXT("Directly playing the attack animation"));
		AttackAnimDuration = Attacker->PlayAnimMontage(Attack);
		FTimerHandle AttackTimerHandle;
		// GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UMelee_LR::FinishAttackTask,
		                                       AttackAnimDuration,
		                                       false);
		return AttackAnimDuration;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DistanceToPlayer is 0!"));
		return 0.0f;
	}
}

void UMelee_LR::Setup(ACharacter* AttackingCharacter)
{
	Attacker = AttackingCharacter;
	AIController = Attacker->GetController<AAIController>();
}

void UMelee_LR::PerformAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Move Completed, playing the attack animation"));
	AttackAnimDuration = Attacker->PlayAnimMontage(Attack);
	FTimerHandle AttackTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UMelee_LR::FinishAttackTask,
											AttackAnimDuration, false);
}

void UMelee_LR::HandleMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	PerformAttack();
}

void UMelee_LR::MoveTowardsPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Starting to move towards player"));
	ACharacter* TargetCharacter{GetWorld()->GetFirstPlayerController()->GetCharacter()};
	if (!IsValid(TargetCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("TargetCharacter is nullptr!"));
		return;
	}
	float DistanceToPlayer{AIController->GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceToPlayer"))};
	if (DistanceToPlayer < MoveAcceptanceRadius)
	{
		PerformAttack();
	}
	FAIMoveRequest MoveRequest{TargetCharacter};
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAcceptanceRadius(MoveAcceptanceRadius);

	AIController->MoveTo(MoveRequest);
	AIController->SetFocus(TargetCharacter);

	AIController->ReceiveMoveCompleted.AddUniqueDynamic(this, &UMelee_LR::HandleMoveCompleted);
}
