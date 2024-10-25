// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerActionsComponent.generated.h"


class UCharacterMovementComponent;
class IMainPlayer;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnSprintSignature, UPlayerActionsComponent, OnSprintDelegate, float,
                                                   StaminaCost);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnRollSignature, UPlayerActionsComponent, OnRollDelegate, float,
                                                   StaminaCost);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONCOMBAT_API UPlayerActionsComponent : public UActorComponent
{
	GENERATED_BODY()

	ACharacter* OwnerCharacter;

	UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(EditAnywhere)
	float SprintStaminaCost{0.1f};

	UPROPERTY(EditAnywhere)
	float SprintSpeed{1000.0f};

	UPROPERTY(VisibleAnywhere)
	float DefaultMaxWalkSpeed{500.0f}; // Temporarily set to 500.0f, will assign the actual value in BeginPlay

	UPROPERTY(EditAnywhere)
	UAnimMontage* RollAnimMontage;

	UPROPERTY(EditAnywhere)
	float RollStaminaCost{5.0f};

	bool bIsRolling{false};

	UPROPERTY(EditAnywhere)
	float InvincibilityDuration{1.0f};

public:
	// Sets default values for this component's properties
	UPlayerActionsComponent();

	FOnSprintSignature OnSprintDelegate;

	FOnRollSignature OnRollDelegate;

	bool bIsInvincible{false};
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Sprinting();

	UFUNCTION(BlueprintCallable)
	void StopSprinting();

	UFUNCTION(BlueprintCallable)
	void Roll();
};
