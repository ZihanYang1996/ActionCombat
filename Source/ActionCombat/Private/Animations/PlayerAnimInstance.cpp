// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PlayerAnimInstance.h"

void UPlayerAnimInstance::UpdateVelocity()
{
	// Get the velocity of the player character, can also use GetOwningActor() instead of TryGetPawnOwner()
	if (APawn* PlayerActor{TryGetPawnOwner()})
	{
		CurrentVelocity = static_cast<float>(PlayerActor->GetVelocity().Length());
	}
}
