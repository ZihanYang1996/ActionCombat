// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/LockonComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULockonComponent::ULockonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	PlayerController = GetWorld()->GetFirstPlayerController();
	// Some alternative ways to get the player controller
	// PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());

	CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
}


// Called every frame
void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsLockedOn && IsValid(CurrentTargetActor))
	{
		// FindLookAtRotation uses FRotationMatrix::MakeFromX(X).Rotator() internally
		FRotator LookAtRotation{
			UKismetMathLibrary::FindLookAtRotation(
				OwnerCharacter->GetActorLocation(),
				CurrentTargetActor->GetActorLocation())
		};
		PlayerController->SetControlRotation(LookAtRotation);
	}
}

void ULockonComponent::ToggleLockon()
{
	if (bIsLockedOn)
	{
		bIsLockedOn = false;
		UE_LOG(LogTemp, Warning, TEXT("Lockon Disabled"));

		// Undo the Lockon
		PlayerController->SetIgnoreLookInput(false);
		CharacterMovementComponent->bOrientRotationToMovement = true;
		CharacterMovementComponent->bUseControllerDesiredRotation = false;

		// Clear the target actor
		CurrentTargetActor = nullptr;

		return;
	}

	FHitResult HitResult;
	FVector CurrentLocation{OwnerCharacter->GetActorLocation()};
	// Only need the location of the actor, as we use a sphere for the collision shape, and the radius is the range of detection
	FCollisionShape Sphere{FCollisionShape::MakeSphere(Radius)};
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(OwnerCharacter); // Ignore the owner of the component

#if 0
	// Alternative way to ignore the owner of the component
	FCollisionQueryParams CollisionParams
	{
		FName{TEXT("Ignore Collision Params")}, // Trace tag
		false, // bTraceComplex
		OwnerCharacter // Ignore actor
	};
#endif


	bool bHasFoundTarget{
		GetWorld()->SweepSingleByChannel(
			HitResult,
			CurrentLocation,
			CurrentLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel1,
			Sphere,
			CollisionParameters)
	};

	if (!bHasFoundTarget)
	{
		return;
	}

	bIsLockedOn = true;
	DrawDebugSphere(GetWorld(), CurrentLocation, Radius, 24, FColor::Green, false, 2.0f);

	// Get the actor that was hit
	CurrentTargetActor = HitResult.GetActor();
	// Prevent the player from looking around
	PlayerController->SetIgnoreLookInput(true);
	// Prevent the character from rotating towards the movement direction
	CharacterMovementComponent->bOrientRotationToMovement = false;
	// Make the character rotate towards the controller's desired rotation
	CharacterMovementComponent->bUseControllerDesiredRotation = true;
}
