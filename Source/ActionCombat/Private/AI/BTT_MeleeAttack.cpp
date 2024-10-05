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
	float DistanceToPlayer{OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceToPlayer"))};

	if (DistanceToPlayer > AttackRadius)
	{
		ACharacter* TargetCharacter{GetWorld()->GetFirstPlayerController()->GetCharacter()};
		if (!IsValid(TargetCharacter))
		{
			return EBTNodeResult::Failed;
		}
		FAIMoveRequest MoveRequest{TargetCharacter};
		MoveRequest.SetUsePathfinding(true);
		MoveRequest.SetAcceptanceRadius(MoveAcceptanceRadius);

		OwnerComp.GetAIOwner()->MoveTo(MoveRequest);
		OwnerComp.GetAIOwner()->SetFocus(TargetCharacter);
		
	}
	
	return EBTNodeResult::Succeeded;
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
