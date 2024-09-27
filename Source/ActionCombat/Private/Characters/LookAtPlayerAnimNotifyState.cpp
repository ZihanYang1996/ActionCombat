// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/LookAtPlayerAnimNotifyState.h"

#include "Characters/LookAtPlayerComponent.h"

void ULookAtPlayerAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (AActor* Owner{MeshComp->GetOwner()})
	{
		if (ULookAtPlayerComponent* LookAtPlayerComponent{Owner->GetComponentByClass<ULookAtPlayerComponent>()})
		{
			LookAtPlayerComponent->bCanRotate = true;
		}
	}
}

void ULookAtPlayerAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
											 const FAnimNotifyEventReference& EventReference)
{
	if (AActor* Owner{MeshComp->GetOwner()})
	{
		if (ULookAtPlayerComponent* LookAtPlayerComponent{Owner->GetComponentByClass<ULookAtPlayerComponent>()})
		{
			LookAtPlayerComponent->bCanRotate = false;
		}
	}
}