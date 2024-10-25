// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerActionsComponent.h"

#include "Characters/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UPlayerActionsComponent::UPlayerActionsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerActionsComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	// Or GetComponentByClass<UCharacterMovementComponent>();
	if (CharacterMovementComponent)
	{
		DefaultMaxWalkSpeed = CharacterMovementComponent->MaxWalkSpeed; // Assign the actual value of MaxWalkSpeed
	}
}


// Called every frame
void UPlayerActionsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerActionsComponent::Sprinting()
{
	if (IMainPlayer::Execute_HasEnoughStamina(OwnerCharacter, SprintStaminaCost))
	{
		if (CharacterMovementComponent->Velocity.Equals(FVector::ZeroVector, 1.0f))
		{
			// If the character is not moving, don't sprint
			return;
		}
		CharacterMovementComponent->MaxWalkSpeed = SprintSpeed;
		OnSprintDelegate.Broadcast(SprintStaminaCost);
	}
	else
	{
		// If the character doesn't have enough stamina, stop sprinting
		StopSprinting();
	}
}

void UPlayerActionsComponent::StopSprinting()
{
	CharacterMovementComponent->MaxWalkSpeed = DefaultMaxWalkSpeed;
}

void UPlayerActionsComponent::Roll()
{
	if (bIsRolling || !IMainPlayer::Execute_HasEnoughStamina(OwnerCharacter, RollStaminaCost))
	{
		return;
	}

	bIsRolling = true;
	bIsInvincible = true;

	OnRollDelegate.Broadcast(RollStaminaCost);

	// Calculate the direction of the roll
	FVector Direction{
		OwnerCharacter->GetCharacterMovement()->Velocity.Length() > 1.0f
			? OwnerCharacter->GetCharacterMovement()->Velocity.GetSafeNormal()
			: OwnerCharacter->GetActorForwardVector()  // Kind of like monster hunter, you have to face the direction you want to roll
	};

	FRotator NewRotation{Direction.Rotation()};

	OwnerCharacter->SetActorRotation(NewRotation);

	float Duration{OwnerCharacter->PlayAnimMontage(RollAnimMontage)};

	auto ResetRollingLambda = [this]()
	{
		bIsRolling = false;
	};

	auto ResetInvincibilityLambda = [this]()
	{
		bIsInvincible = false;
	};

	FTimerHandle RollingTimerHandle;
	FTimerHandle InvincibilityTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(RollingTimerHandle, ResetRollingLambda, Duration, false);
	GetWorld()->GetTimerManager().SetTimer(InvincibilityTimerHandle, ResetInvincibilityLambda, InvincibilityDuration, false);
}
