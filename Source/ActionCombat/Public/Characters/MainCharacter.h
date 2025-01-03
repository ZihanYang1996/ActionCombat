// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Fighter.h"
#include "Interfaces/MainPlayer.h"
#include "MainCharacter.generated.h"

class UPlayerActionsComponent;
class UBlockComponent;
class UTraceComponent;
class UCombatComponent;
class ULockonComponent;
class UStatsComponent;
class UPlayerAnimInstance;

UCLASS()
class ACTIONCOMBAT_API AMainCharacter : public ACharacter, public IMainPlayer, public IFighter
{
	GENERATED_BODY()

	UFUNCTION()
	void OnDamageReceived(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HurtAnimMontage;
	
public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStatsComponent* StatsComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULockonComponent* LockonComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCombatComponent* CombatComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTraceComponent* TraceComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBlockComponent* BlockComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPlayerActionsComponent* PlayerActionsComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> HitShake;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	UPlayerAnimInstance* PlayerAnimInstance;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float GetDamage() override;

	virtual bool HasEnoughStamina_Implementation(float StaminaCost) override;

	UFUNCTION(BlueprintCallable)
	void HandleDeath();

	virtual void AbortLockon(AActor* TargetActor) override;

	UFUNCTION(BlueprintCallable)
	void StartBlock();

	UFUNCTION(BlueprintCallable)
	void EndBlock();

	virtual bool CanTakeDamage(AActor* DamageCauser) const override;

	virtual bool CanTakeInput() override;

	virtual void InputReset() override;

	void StartHitPause(float Duration, float Ratio);
};
