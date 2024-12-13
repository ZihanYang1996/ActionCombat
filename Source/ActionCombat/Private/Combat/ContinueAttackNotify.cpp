// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/ContinueAttackNotify.h"

#include "GameFramework/Character.h"

void UContinueAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ACharacter* Character{Cast<ACharacter>(MeshComp->GetOwner())})
	{
		UE_LOG(LogTemp, Warning, TEXT("ContinueAttackNotify::Notify"));
		Character->PlayAnimMontage(NextMontage);
	}
}
