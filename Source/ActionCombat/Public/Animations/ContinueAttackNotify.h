// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ContinueAttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API UContinueAttackNotify : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* NextMontage;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	void SetNetMontage(UAnimMontage* Montage) { NextMontage = Montage; }
	
};
