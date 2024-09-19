// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraceComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONCOMBAT_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere)
	FName StartSocket;

	UPROPERTY(EditAnywhere)
	FName EndSocket;

	UPROPERTY(EditAnywhere)
	FName Rotation;

	UPROPERTY(EditAnywhere)
	double BoxCollisionEdgeLength{30.0f};

	UPROPERTY(EditAnywhere)
	bool bIsInDebugMode{false};

	TMap<AActor*, int32> DamagedActors;

	// Number of hits that single attack can do
	UPROPERTY(EditAnywhere)
	int32 MaxHits{1};

public:
	// Sets default values for this component's properties
	UTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void EmptyDamagedActors();
};
