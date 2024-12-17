// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttackMove.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ACTIONCOMBAT_API UAttackMove : public UObject
{
	GENERATED_BODY()

public:
	virtual float Execute() { return 0.0f; };

	virtual void Setup(ACharacter* AttackingCharacter)
	{
	};

	virtual void FinishAttackTask()
	{
	};
	
	virtual ~UAttackMove();
};
