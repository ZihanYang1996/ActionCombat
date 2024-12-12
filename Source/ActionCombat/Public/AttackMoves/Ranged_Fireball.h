// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackMoves/AttackMove.h"
#include "Ranged_Fireball.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONCOMBAT_API URanged_Fireball : public UAttackMove
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* RangedAttack;

	ACharacter* Attacker;

public:
	virtual float Execute() override;

	virtual void Setup(ACharacter* AttackingCharacter) override;
};
