// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TraceNotifyState.h"

#include "Combat/TraceComponent.h"
#include "GameFramework/Character.h"

void UTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                    const FAnimNotifyEventReference& EventReference)
{
	if (!IsValid(MeshComp->GetOwner()))
	{
		return;
	}
	if (UTraceComponent* TraceComp = MeshComp->GetOwner()->GetComponentByClass<UTraceComponent>())
	{
		TraceComp->TraceStart();
	}
}

void UTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	if (!IsValid(MeshComp->GetOwner()))
	{
		return;
	}
	if (UTraceComponent* TraceComp = MeshComp->GetOwner()->GetComponentByClass<UTraceComponent>())
	{
		TraceComp->TraceReset();
	}
}
