// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
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

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	AAIController* AIControllerPtr;

	ACharacter* CharacterPtr;

	UBossAnimInstance* BossAnimInstancePtr;

	void ChargeAtPlayer();

	UPROPERTY(EditAnywhere)
	float AcceptanceRadius{100.0f};

	void FinishAttackTask();
	
	UFUNCTION()
	void HandleMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	float OriginalWalkSpeed;
	float ChargeWalkSpeed{2000.0f};

public:
	UBTT_ChargeAttack();
};
