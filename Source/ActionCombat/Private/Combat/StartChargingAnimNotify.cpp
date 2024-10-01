// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/StartChargingAnimNotify.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

void UStartChargingAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (ACharacter* OwnerCharacter{Cast<ACharacter>(MeshComp->GetOwner())})
	{
		if (AAIController* OwnerAIController{Cast<AAIController>(OwnerCharacter->GetController())})
		{
			OwnerAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsReadyToCharge"), true);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OwnerAIController is not valid"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OwnerCharacter is not valid"));
	}
}
