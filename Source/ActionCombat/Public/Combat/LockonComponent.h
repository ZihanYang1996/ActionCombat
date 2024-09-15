// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockonComponent.generated.h"


class USpringArmComponent;
class UCharacterMovementComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONCOMBAT_API ULockonComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULockonComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ToggleLockon();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius{1000.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double BreakDistance{2000.0f};

	AActor* CurrentTargetActor;

private:
	bool bIsLockedOn{false};

	ACharacter* OwnerCharacter;

	APlayerController* PlayerController;

	UCharacterMovementComponent* CharacterMovementComponent;

	USpringArmComponent* SpringArmComponent;

	void StartLockon(FHitResult HitResult);

	void EndLockon();
};