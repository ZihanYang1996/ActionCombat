// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackMoves/AttackMove.h"
#include "Charge_FromSky.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONCOMBAT_API UCharge_FromSky : public UAttackMove
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* PreAttack_Montage;

	float PreAttackDuration{};

	UPROPERTY(EditAnywhere)
	UAnimMontage* RiseToSky_Montage;

	float RiseDuration{};

	UPROPERTY(EditAnywhere)
	UAnimMontage* FallAndAttack_Montage;

	float FallDuration{};

	UPROPERTY(EditAnywhere)
	float RiseHeight{1000.0f};

	FVector LocationAfterRise{};

	FVector LandingLocation{};

	float RiseSpeed{};
	
	UPROPERTY(EditAnywhere)
	float FallSpeed{5.0f};
	
	FTimerHandle TimerHandle;

	ACharacter* Attacker{};
	
	void CharacterRise();
	void CharacterFall();
	void RestartAI();

	const float TimeInterval = 0.001f;

public:
	virtual void Setup(ACharacter* AttackingCharacter) override;
	virtual float Execute() override;
	
};
