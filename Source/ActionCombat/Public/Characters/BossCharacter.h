// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EEnemyState.h"
#include "GameFramework/Character.h"
#include "Interfaces/Enemy.h"
#include "Interfaces/Fighter.h"
#include "BossCharacter.generated.h"

class UCombatComponent;
class UBlackboardComponent;
class UStatsComponent;

UCLASS()
class ACTIONCOMBAT_API ABossCharacter : public ACharacter, public IEnemy, public IFighter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere)
	UCombatComponent* CombatComponent;

	UFUNCTION()
	void HandlePlayerDeath();

public:
	// Sets default values for this character's properties
	ABossCharacter();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStatsComponent* StatsComponent;

	UPROPERTY(EditAnywhere)
	EEnemyState InitialState;
	UBlackboardComponent* BlackboardComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// virtual void OnSelected_Implementation() override;

	UFUNCTION()
	void DamageReceived(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
									 AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	void DetectPawn(APawn* PawnDetected, APawn* PawnToDetect) const;

	virtual float GetDamage() override;

	virtual void Attack() override;

	virtual float GetAnimDuration() const override;

	virtual float GetMeleeRange() const override;
};
