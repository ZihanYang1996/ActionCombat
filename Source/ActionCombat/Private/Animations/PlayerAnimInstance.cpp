// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PlayerAnimInstance.h"

void UPlayerAnimInstance::UpdateSpeed()
{
	// Get the velocity of the player character, can also use GetOwningActor() instead of TryGetPawnOwner()
	if (APawn* PlayerPawn{TryGetPawnOwner()})
	{
		CurrentSpeed = static_cast<float>(PlayerPawn->GetVelocity().Length());
	}
}

void UPlayerAnimInstance::HandleUpdatedTarget(AActor* NewTarget)
{
	bIsInCombat = IsValid(NewTarget);
}

void UPlayerAnimInstance::UpdateDirection()
{
	if (!bIsInCombat)
	{
		return;
	}
	if (APawn* PlayerPawn{TryGetPawnOwner()})
	{
		// Get the direction of the player character
		// First get the transform of the player character
		FTransform PlayerTransform{PlayerPawn->GetActorTransform()};
		// Then use the InverseTransformVector to get the direction (vector) of the player character
		FVector PlayerDirection{PlayerTransform.InverseTransformVector(PlayerPawn->GetVelocity())};
		// Then use the Vector to Rotator to get the rotation of the player character
		FRotator PlayerDirectionRotator{PlayerDirection.Rotation()};
		// Then get the Yaw of the player character and assign it to the CurrentDirection
		CurrentDirection = PlayerDirectionRotator.Yaw;
		
		// Or use the CalculateDirection function, which is a built-in function in AnimInstance
		// CurrentDirection = CalculateDirection(PlayerPawn->GetVelocity(), PlayerPawn->GetActorRotation());
	} 
}
