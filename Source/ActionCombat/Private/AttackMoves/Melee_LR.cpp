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
		// Play the pre-attack animation with a probability of CurrentPreAttackProbability
		float RandomVlaue{FMath::FRand()};
		if (RandomVlaue < CurrentPreAttackProbability)
		{
			PreAttackAnimDuration = Attacker->PlayAnimMontage(PreAttack);
			FTimerHandle PreAttackTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(PreAttackTimerHandle, this, &UMelee_LR::MoveTowardsPlayer,
												   PreAttackAnimDuration,
												   false);
			// Set the CurrentPreAttackProbability to 0, so the pre-attack animation is never played twice
			CurrentPreAttackProbability = 0;
		}
		else
		{
			PreAttackAnimDuration = 0.0f;
			MoveTowardsPlayer();
		}
		return 1.0f;
	}
	else if (DistanceToPlayer != 0.0f)
	{
		// Play the pre-attack animation with a probability of CurrentPreAttackProbability
		float RandomVlaue{FMath::FRand()};
		if (RandomVlaue < CurrentPreAttackProbability)
		{
			PreAttackAnimDuration = Attacker->PlayAnimMontage(PreAttack);
			FTimerHandle PreAttackTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(PreAttackTimerHandle, this, &UMelee_LR::PerformAttack,
												   PreAttackAnimDuration,
												   false);
			// Set the CurrentPreAttackProbability to 0, so the pre-attack animation is never played twice
			CurrentPreAttackProbability = 0;
		}
		else
		{
			PreAttackAnimDuration = 0.0f;
			PerformAttack();
		}
		return 1.0f;
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
	CurrentPreAttackProbability = PreAttackProbability;
}

void UMelee_LR::PerformAttack()
{
	AttackAnimDuration = Attacker->PlayAnimMontage(Attack);
	FTimerHandle AttackTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UMelee_LR::FinishAttackTask,
											AttackAnimDuration, false);
	// Reset the CurrentPreAttackProbability to the original value
	CurrentPreAttackProbability = PreAttackProbability;
}

void UMelee_LR::HandleMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	PerformAttack();
}

void UMelee_LR::MoveTowardsPlayer()
{
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
