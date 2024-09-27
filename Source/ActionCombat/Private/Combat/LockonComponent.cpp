// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/LockonComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Enemy.h"
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

	SpringArmComponent = OwnerCharacter->FindComponentByClass<USpringArmComponent>();
}


// Called every frame
void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsLockedOn && IsValid(CurrentTargetActor))
	{
		FVector StartLocation{OwnerCharacter->GetActorLocation()};
		FVector EndLocation{CurrentTargetActor->GetActorLocation()};

		// Check if the target is out of range
		double Distance{FVector::Distance(StartLocation, EndLocation)};
		if (Distance > BreakDistance)
		{
			EndLockon();
			return;
		}

		EndLocation.Z -= 150.0f; // Make the rotation looks downwards a bit
		// FindLookAtRotation uses FRotationMatrix::MakeFromX(Target - Start).Rotator() internally
		FRotator LookAtRotation{UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation)};

		PlayerController->SetControlRotation(LookAtRotation);
	}
}


void ULockonComponent::ToggleLockon()
{
	if (bIsLockedOn)
	{
		// If the lockon is currently enabled, disable it
		EndLockon();
		return;
	}

	// If the lockon is currently disabled, enable it

	// Perform a sphere trace to find the target
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

	// If the trace didn't hit anything, return
	if (!bHasFoundTarget || !HitResult.GetActor()->Implements<UEnemy>())
	{
		return;
	}

	// If the actor that was hit doesn't implement the UEnemy interface, return
	if (!HitResult.GetActor()->Implements<UEnemy>())
	{
		return;
	}


	DrawDebugSphere(GetWorld(), CurrentLocation, Radius, 24, FColor::Green, false, 2.0f);

	StartLockon(HitResult);
}


void ULockonComponent::StartLockon(FHitResult HitResult)
{
	bIsLockedOn = true;
	// Get the actor that was hit
	CurrentTargetActor = HitResult.GetActor();
	// Prevent the player from looking around
	PlayerController->SetIgnoreLookInput(true);
	// Prevent the character from rotating towards the movement direction
	CharacterMovementComponent->bOrientRotationToMovement = false;
	// Make the character rotate towards the controller's desired rotation
	CharacterMovementComponent->bUseControllerDesiredRotation = true;
	// Adjust the spring arm's target offset to raise the camera a bit
	SpringArmComponent->TargetOffset = FVector{0.0f, 0.0f, 100.0f};
	// Call the OnSelected function of the enemy
	IEnemy::Execute_OnSelected(CurrentTargetActor);
	// Broadcast the event that the target has been updated
	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
}


void ULockonComponent::EndLockon()
{
	bIsLockedOn = false;
	UE_LOG(LogTemp, Warning, TEXT("Lockon Disabled"));

	// Undo the Lockon
	// Not using SetIgnoreLookInput(false) because camera can be locked multiple times 
	PlayerController->ResetIgnoreLookInput();
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->bUseControllerDesiredRotation = false;

	// Call the OnDeselected function of the enemy
	IEnemy::Execute_OnDeselected(CurrentTargetActor);

	// Clear the target actor
	CurrentTargetActor = nullptr;

	// Undo the spring arm's target offset
	SpringArmComponent->TargetOffset = FVector::ZeroVector;

	// Broadcast the event that the target has been updated
	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
}
