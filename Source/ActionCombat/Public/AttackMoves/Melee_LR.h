// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackMoves/AttackMove.h"
#include "Melee_LR.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONCOMBAT_API UMelee_LR : public UAttackMove
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* PreAttack;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* Attack;

	float TotalDuration;

	ACharacter* Attacker;

public:
	virtual float Execute() override;

	virtual void Setup(ACharacter* AttackingCharacter) override;
};
