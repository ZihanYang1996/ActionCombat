// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnAttackPerformedSignature, UCombatComponent, OnAttackPerformedDelegate, float, AttackStaminaCost);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONCOMBAT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> AttackMontages;

	ACharacter* OwnerCharacter;

	UPROPERTY(VisibleAnywhere)
	int CurrentAttackMontageIndex{0};

	int MaxAttackMontageIndex;

	bool bCanContinueCombo{false};

	bool bIsAttacking{false};
	
	UPROPERTY(EditAnywhere)
	float ComboWindowTime{0.5f};

	UPROPERTY(EditAnywhere)
	float AttackStaminaCost{5.0f};

public:
	// Sets default values for this component's properties
	UCombatComponent();

	void EnableComboContinuation();

	void ResetCombo();

	FOnAttackPerformedSignature OnAttackPerformedDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void CombatAttack();

	UFUNCTION(BlueprintCallable)
	void RandomAttack();
};
