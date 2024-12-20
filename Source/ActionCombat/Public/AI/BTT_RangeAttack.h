// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API UBTT_RangeAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void FinishAttackTask();
	
	bool bIsFinished{false};

	UPROPERTY(EditAnywhere)
	float ChargeAttackTransitionThreshold{0.8f};
	float CurrentChargeAttackTransitionThreshold{ChargeAttackTransitionThreshold};

public:
	UBTT_RangeAttack();
};
