// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ChargeAttack.generated.h"

class UBossAnimInstance;
/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API UBTT_ChargeAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	AAIController* AIControllerPtr;

	ACharacter* CharacterPtr;

	UBossAnimInstance* BossAnimInstancePtr;

	// UPROPERTY(EditAnywhere)
	// UAnimMontage* ChargeAttack;
};
