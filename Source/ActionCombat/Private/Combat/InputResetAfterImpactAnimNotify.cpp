// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/InputResetAfterImpactAnimNotify.h"

#include "Interfaces/MainPlayer.h"

void UInputResetAfterImpactAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp->GetOwner()->Implements<UMainPlayer>())
	{
		Cast<IMainPlayer>(MeshComp->GetOwner())->InputReset();
	}
}
