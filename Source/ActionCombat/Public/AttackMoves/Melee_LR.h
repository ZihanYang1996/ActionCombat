// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackMoves/AttackMove.h"
#include "Melee_LR.generated.h"

struct FAIRequestID;

namespace EPathFollowingResult
{
	enum Type : int;
}

class AAIController;
/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONCOMBAT_API UMelee_LR : public UAttackMove
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* PreAttack;

	float PreAttackAnimDuration{};
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* Attack;

	float AttackAnimDuration{};
	
	ACharacter* Attacker;

	AAIController* AIController;

	UFUNCTION()
	void MoveTowardsPlayer();

	UPROPERTY(EditAnywhere)
	float AttackRadius{200.0f};
	
	UPROPERTY(EditAnywhere)
	float MoveAcceptanceRadius{100.0f};

	virtual void FinishAttackTask() override;

public:
	virtual float Execute() override;

	virtual void Setup(ACharacter* AttackingCharacter) override;

	UFUNCTION()
	void HandleMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
};
