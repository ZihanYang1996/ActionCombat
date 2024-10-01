// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_ChargeAttack.h"

#include "AIController.h"
#include "Animations/BossAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
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
}

void UBTT_ChargeAttack::ChargeAtPlayer()
{
	APawn* TargetPawn{GetWorld()->GetFirstPlayerController()->GetCharacter()};
	if (IsValid(TargetPawn))
	{
		FAIMoveRequest MoveRequest{TargetPawn};
		// pathfinding: if set - regular pathfinding will be used, if not - direct path between two points
		MoveRequest.SetUsePathfinding(true);
		// acceptance radius: the distance at which the AI will be considered to have reached the target
		MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

		// Move the AI to the player
		AIControllerPtr->MoveTo(MoveRequest);
		// Aim at the player
		AIControllerPtr->SetFocus(TargetPawn);
	}
}
