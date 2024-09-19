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

	if (!IsValid(MeshComp))
	{
		return;
	}
	if (ACharacter* OwningCharacter = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		OwningCharacter->GetComponentByClass<UCombatComponent>()->EnableComboContinuation();
		// Clear damaged actors, which contorls how many times an actor can be damaged in a single attack 
		OwningCharacter->GetComponentByClass<UTraceComponent>()->EmptyDamagedActors();
	}
}

void UComboContinuationAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!IsValid(MeshComp))
	{
		return;
	}
	if (ACharacter* OwningCharacter = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		// Disable combo continuation
		OwningCharacter->GetComponentByClass<UCombatComponent>()->ResetCombo();
	}
}
