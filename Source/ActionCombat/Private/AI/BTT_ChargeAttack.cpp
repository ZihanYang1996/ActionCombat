// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_ChargeAttack.h"

#include "AIController.h"
#include "Animations/BossAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_ChargeAttack::UBTT_ChargeAttack()
{
	NodeName = TEXT("Charge Attack");
	bNotifyTick = true; // This will make the TickTask function be called
}

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIControllerPtr = OwnerComp.GetAIOwner();

	CharacterPtr = AIControllerPtr->GetCharacter();

	BossAnimInstancePtr = Cast<UBossAnimInstance>(CharacterPtr->GetMesh()->GetAnimInstance());

	BossAnimInstancePtr->bIsCharging = true;

	

	// Set the IsReadyToCharge key to false first, because there are some animations that need to be played before the charge
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsReadyToCharge"), false);

	bIsFinished = false;
	return EBTNodeResult::InProgress;
}

void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	bool bIsReadyToCharge{OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsReadyToCharge"))};
	if (bIsReadyToCharge)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsReadyToCharge"), false);
		ChargeAtPlayer();
	}
	if (bIsFinished)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		AIControllerPtr->ReceiveMoveCompleted.RemoveDynamic(this, &UBTT_ChargeAttack::HandleMoveCompleted);
	}
}

void UBTT_ChargeAttack::ChargeAtPlayer()
{
	APawn* TargetPawn{GetWorld()->GetFirstPlayerController()->GetCharacter()};
	if (IsValid(TargetPawn))
	{
		// Boost the AI's speed
		OriginalWalkSpeed = CharacterPtr->GetCharacterMovement()->MaxWalkSpeed;
		CharacterPtr->GetCharacterMovement()->MaxWalkSpeed = ChargeWalkSpeed;
		
		FAIMoveRequest MoveRequest{TargetPawn};
		// pathfinding: if set - regular pathfinding will be used, if not - direct path between two points
		MoveRequest.SetUsePathfinding(true);
		// acceptance radius: the distance at which the AI will be considered to have reached the target
		MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

		// Move the AI to the player
		AIControllerPtr->MoveTo(MoveRequest);
		// Aim at the player
		AIControllerPtr->SetFocus(TargetPawn);

		// Bind the HandleMoveCompleted function to the ReceiveMoveCompleted event
		AIControllerPtr->ReceiveMoveCompleted.AddUniqueDynamic(this, &UBTT_ChargeAttack::HandleMoveCompleted);
	}
}

// Function will be called when the AI has reached the player
void UBTT_ChargeAttack::HandleMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	BossAnimInstancePtr->bIsCharging = false;

	// Create a timer
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBTT_ChargeAttack::FinishAttackTask, 1.0f, false);

	// Reset the AI's speed
	CharacterPtr->GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
}

void UBTT_ChargeAttack::FinishAttackTask()
{
	UE_LOG(LogTemp, Warning, TEXT("FinishAttackTask"));
	bIsFinished = true;
}