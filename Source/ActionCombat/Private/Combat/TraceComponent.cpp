// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TraceComponent.h"

#include "Characters/MainCharacter.h"
#include "Engine/DamageEvents.h"
#include "Interfaces/Fighter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	SkeletalMeshComponent = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
}


// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsAttacking)
	{
		return;
	}

	// Create a TArray of FHitResult to store the all the results of the traces
	TArray<FHitResult> AllOutResults;

	// Iterate over all the TraceSocket structs
	for (const FTraceSockets& TraceSockets : TraceSocketsArray)
	{
		FVector StartSocketLocation{SkeletalMeshComponent->GetSocketLocation(TraceSockets.StartSocket)};

		FVector EndSocketLocation{SkeletalMeshComponent->GetSocketLocation(TraceSockets.EndSocket)};

		FQuat ShapeRotation{SkeletalMeshComponent->GetSocketQuaternion(TraceSockets.Rotation)};

		FVector WeaponCenter{(StartSocketLocation + EndSocketLocation) / 2.0f};

		TArray<FHitResult> OutResults;

		double WeaponLength{FVector::Distance(StartSocketLocation, EndSocketLocation)};

		FVector BoxHalfExtent{BoxCollisionEdgeLength, BoxCollisionEdgeLength, WeaponLength};
		BoxHalfExtent /= 2.0f;

		FCollisionShape Box{FCollisionShape::MakeBox(BoxHalfExtent)};

		FCollisionQueryParams CollisionParams{
			FName{TEXT("Ignore Collision Params")},
			false,
			GetOwner()
		};

		bool bHasFoundTargets{
			GetWorld()->SweepMultiByChannel(
				OutResults,
				WeaponCenter,
				WeaponCenter,
				ShapeRotation,
				ECollisionChannel::ECC_GameTraceChannel1,
				Box,
				CollisionParams
			)
		};

		// Add all the results to the AllOutResults array
		// for (const FHitResult& HitResult : OutResults)
		// {
		// 	AllOutResults.Add(HitResult);
		// }
		AllOutResults.Append(OutResults);

		if (bIsInDebugMode)
		{
			DrawDebugBox(GetWorld(),
			             WeaponCenter,
			             Box.GetExtent(),
			             ShapeRotation,
			             bHasFoundTargets ? FColor::Green : FColor::Red,
			             false,
			             1.0f);
		}

		// Do an extra trace between the previous and current end socket
		if (PreviousEndSocketLocations.Contains(TraceSockets.EndSocket))
		{
			TArray<FHitResult> OutResultsExtra;
			
			FVector PreviousEndSocketLocation{PreviousEndSocketLocations[TraceSockets.EndSocket]};
			
			FVector TraceCenter{(PreviousEndSocketLocation + EndSocketLocation) / 2.0f};
			
			double TraceLength{FVector::Distance(PreviousEndSocketLocation, EndSocketLocation)};
			FVector BoxHalfExtentExtra{TraceLength, BoxCollisionEdgeLength, BoxCollisionEdgeLength};
			BoxHalfExtentExtra /= 2.0f;
			FCollisionShape BoxExtra{FCollisionShape::MakeBox(BoxHalfExtentExtra)};
			
			FVector Direction{EndSocketLocation - PreviousEndSocketLocation};
			FRotator Rotation{Direction.Rotation()};
			FQuat ShapeRotationExtra{Rotation};
			
			bool bHasFoundTargetsExtra{
				GetWorld()->SweepMultiByChannel(
					OutResultsExtra,
					TraceCenter,
					TraceCenter,
					ShapeRotationExtra,
					ECollisionChannel::ECC_GameTraceChannel1,
					BoxExtra,
					CollisionParams
					)
			};

			AllOutResults.Append(OutResultsExtra);
			
			if (bIsInDebugMode)
			{
				DrawDebugBox(GetWorld(),
							 TraceCenter,
							 BoxExtra.GetExtent(),
							 ShapeRotationExtra,
							 bHasFoundTargetsExtra ? FColor::Green : FColor::Red,
							 false,
							 1.0f);
			}
			
		}
		
		PreviousEndSocketLocations.Add(TraceSockets.EndSocket, EndSocketLocation);
	}
	


	// Check if we have found any targets
	if (AllOutResults.Num() == 0)
	{
		return;
	}

	// Calculate the damage
	float CharacterDamage{0.0f};
	IFighter* Fighter{Cast<IFighter>(GetOwner())};
	if (Fighter)
	{
		CharacterDamage = Fighter->GetDamage();
	}

	FDamageEvent DamageEvent{};
	AController* InstigatorController{GetOwner()->GetInstigatorController()};

	for (const FHitResult& HitResult : AllOutResults)
	{
		AActor* HitActor{HitResult.GetActor()};

		// Check if the actor has already been damaged
		// If it has, check if it can be damaged again
		// If it can't, continue to the next actor
		if (!DamagedActors.Contains(HitActor))
		{
			DamagedActors.Add(HitActor, MaxHits);
		}
		else if (DamagedActors[HitActor] == 0)
		{
			continue;
		}

		DamagedActors[HitActor]--;

		// Apply damage to the hit actor, both approaches are valid
		// HitActor->TakeDamage(CharacterDamage, DamageEvent, InstigatorController, GetOwner());
		UGameplayStatics::ApplyDamage(HitActor, CharacterDamage, InstigatorController, GetOwner(),
		                              UDamageType::StaticClass());

		// Add blood particles
		if (HitParticleTemplate)
		{
			// Only play the particle effect if the HitActor is of type Fighter
			// and the HitActor can take damage
			if (IFighter* HitActorFighter{Cast<IFighter>(HitActor)})
			{
				
				if (!HitActorFighter->CanTakeDamage(GetOwner()))
				{
					continue;
				}
				FVector Location{HitResult.ImpactPoint};
				FRotator Rotation{HitResult.ImpactNormal.Rotation()};
				Rotation.Pitch += 90.0f;
				UGameplayStatics::SpawnEmitterAttached(HitParticleTemplate, HitResult.GetComponent(), NAME_None, Location,
													   Rotation, EAttachLocation::KeepWorldPosition,
													   true);
			}

		}

		// Apply hit pause
		// GetOwner<AMainCharacter>()->StartHitPause(0.1, 0.05);
		// Only call StartHitPause if the owner is a MainCharacter
		if (AMainCharacter* MainCharacter{Cast<AMainCharacter>(GetOwner())})
		{
			MainCharacter->StartHitPause(0.1, 0.05);
		}
	}
}

void UTraceComponent::TraceStart()
{
	bIsAttacking = true;
}

// Clear damaged actors, which contorls how many times an actor can be damaged in a single attack
// Also set bIsAttacking to false, so tracing is stopped
void UTraceComponent::TraceReset()
{
	bIsAttacking = false;
	DamagedActors.Empty();
}
