// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyProjectile.generated.h"

class USphereComponent;

UCLASS()
class ACTIONCOMBAT_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()

	USphereComponent* SphereComponent;
	UParticleSystemComponent* ParticleSystemComponent;

	UFUNCTION()
	void DestoryProjectile();

	FTimerHandle DestroyTimerHandle;
	
public:	
	// Sets default values for this actor's properties
	AEnemyProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	UParticleSystem* ExplosionParticle;

};
