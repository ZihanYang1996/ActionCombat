// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/ComboContinuationAnimNotifyState.h"

#include "Combat/CombatComponent.h"
#include "Combat/TraceComponent.h"
#include "GameFramework/Character.h"

void UComboContinuationAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                    float TotalDuration,
                                                    const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (UCombatComponent* CombatComp = MeshComp->GetOwner()->GetComponentByClass<UCombatComponent>())
	{
		CombatComp->EnableComboContinuation();
	}
}

void UComboContinuationAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (UCombatComponent* CombatComp = MeshComp->GetOwner()->GetComponentByClass<UCombatComponent>())
	{
		CombatComp->ResetCombo();
	}
}
