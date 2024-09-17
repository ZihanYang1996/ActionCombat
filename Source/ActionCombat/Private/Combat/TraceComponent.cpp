// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TraceComponent.h"

#include "Interfaces/Fighter.h"

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

	FVector StartSocketLocation{SkeletalMeshComponent->GetSocketLocation(StartSocket)};

	FVector EndSocketLocation{SkeletalMeshComponent->GetSocketLocation(EndSocket)};

	FQuat ShapeRotation{SkeletalMeshComponent->GetSocketQuaternion(Rotation)};

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

	// Check if we have found any targets
	if (OutResults.Num() == 0)
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
	UE_LOG(LogTemp, Warning, TEXT("Character damage: %f"), CharacterDamage);
}
