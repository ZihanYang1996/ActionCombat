// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTS_PlayerDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


UBTS_PlayerDistance::UBTS_PlayerDistance()
{
	NodeName = TEXT("Player Distance");
	bNotifyTick = true;
	bNotifyBecomeRelevant = true;
}

void UBTS_PlayerDistance::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();
}

void UBTS_PlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is nullptr!"));
		return;
	}
	FVector PlayerLocation{PlayerCharacter->GetActorLocation()};
	float DistanceToPlayer{
		static_cast<float>(FVector::Distance(PlayerLocation, OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation()))
	};

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("DistanceToPlayer"), DistanceToPlayer);
}
