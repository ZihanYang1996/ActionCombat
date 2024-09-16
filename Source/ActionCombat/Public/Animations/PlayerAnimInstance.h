// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentSpeed{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentDirection{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInCombat{false};

	UFUNCTION(BlueprintCallable)
	void UpdateSpeed();

	UFUNCTION(BlueprintCallable)
	void UpdateDirection();

public:
	UFUNCTION(BlueprintCallable)
	void HandleUpdatedTarget(AActor* NewTarget);
};
