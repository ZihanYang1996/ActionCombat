// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/EnemyProjectileComponent.h"

// Sets default values for this component's properties
UEnemyProjectileComponent::UEnemyProjectileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyProjectileComponent::BeginPlay()
{
	Super::BeginPlay();
	ProjectileSpawnPoint = Cast<USceneComponent>(GetOwner()->GetDefaultSubobjectByName(ProjectileSpawnPointName));
}


// Called every frame
void UEnemyProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemyProjectileComponent::SpawnProjectile()
{
	if (!IsValid(ProjectileSpawnPoint))
	{
		UE_LOG(LogTemp, Error, TEXT("Projectile spawn point is not valid!"));
		return;
	}
	FVector SpawnLocation{ProjectileSpawnPoint->GetComponentLocation()};

	// Calculate the rotation of the projectile
	FVector PlayerLocation{GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()};
	// Option 1:
	// FRotator SpawnRotation{FVector{PlayerLocation - SpawnLocation}.GetSafeNormal().Rotation()};
	// Option 2:
	// FRotator SpawnRotation{UKismetMathLibrary::FindLookAtRotation(SpawnLocation, PlayerLocation)};
	// Option 3:
	FRotator SpawnRotation{FRotationMatrix::MakeFromX(PlayerLocation - SpawnLocation).Rotator()};

	AActor* SpawnedProjectile{GetWorld()->SpawnActor(ProjectileClass, &SpawnLocation, &SpawnRotation)};
	SpawnedProjectile->SetOwner(GetOwner());
}
