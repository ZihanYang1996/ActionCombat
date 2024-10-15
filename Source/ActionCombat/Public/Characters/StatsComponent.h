// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ECharacterStat.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnZeroHealthSignature, UStatsComponent, OnZeroHealthDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONCOMBAT_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	double StaminaRegenRate{10.0f};

	FTimerHandle StaminaRegenTimer;

	void EnableStaminaRegen();

	bool bCanStaminaRegen{true};

	float StaminaDelayDuration{2.0f};

public:
	// Sets default values for this component's properties
	UStatsComponent();

	UPROPERTY(EditAnywhere)
	TMap<ECharacterStat, float> Stats;

	void ReduceHealth(float Amount);

	UFUNCTION()
	void ReduceStamina(float Amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void RegenerateStamina();

	UFUNCTION(BlueprintPure)
	float GetStatPercentage(ECharacterStat Stat, ECharacterStat MaxStat) const;

	UPROPERTY(BlueprintAssignable)
	FOnZeroHealthSignature OnZeroHealthDelegate;
};
