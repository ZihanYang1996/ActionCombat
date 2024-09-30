// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/EnemyProjectile.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent = GetComponentByClass<USphereComponent>();
	ParticleSystemComponent = GetComponentByClass<UParticleSystemComponent>();

	if (IsValid(SphereComponent))
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::HandleBeginOverlap);
	}
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyProjectile::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
										 const FHitResult& SweepResult)
{
	APawn* OtherPawn{Cast<APawn>(OtherActor)};

	if (!OtherPawn->IsPlayerControlled())
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Projectile overlapped with %s"), *OtherActor->GetName());

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (IsValid(ParticleSystemComponent))
	{
		ParticleSystemComponent->SetTemplate(ExplosionParticle);
		ParticleSystemComponent->Activate();
	}

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AEnemyProjectile::DestoryProjectile, 1.0f);
}

void AEnemyProjectile::DestoryProjectile()
{
	Destroy();
}
