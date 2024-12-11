// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackMoves/Charge_FromSky.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharge_FromSky::Setup(ACharacter* AttackingCharacter)
{
	RiseDuration = RiseToSky_Montage->GetPlayLength();
	RiseSpeed = RiseHeight / RiseDuration;
	FallDuration = FallAndAttack_Montage->GetPlayLength();

	this->Attacker = AttackingCharacter;
}

void UCharge_FromSky::Execute(ACharacter* Character, float& Duration)
{
	Duration = RiseDuration + FallDuration + 0.5;  // 2 for some extra time
	// First, pause the control from AI
	AAIController* AIController{Cast<AAIController>(Character->GetController())};
	if (AIController)
	{
		AIController->StopMovement();
		AIController->BrainComponent->PauseLogic("Temporarily stopped, charging from the sky");
	}
	Character->GetCharacterMovement()->DisableMovement();

	// Rise to the sky while playing the RiseToSky_Montage animation

	LocationAfterRise = Character->GetActorLocation();
	LocationAfterRise.Z += RiseHeight;
	
	Character->PlayAnimMontage(RiseToSky_Montage);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCharge_FromSky::CharacterRise, TimeInterval, true);
}

void UCharge_FromSky::CharacterRise()
{
	UE_LOG(LogTemp, Warning, TEXT("Rising"));
	FVector CurrenLocation{Attacker->GetActorLocation()};
	if (CurrenLocation.Equals(LocationAfterRise, 10.0f))
	{
		Attacker->SetActorLocation(LocationAfterRise);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		LandingLocation = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetActorLocation();  // Get the player's location
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCharge_FromSky::CharacterFall, TimeInterval, true);
		return;	
	}
	
	FVector NewLocation{FMath::VInterpConstantTo(CurrenLocation, LocationAfterRise, TimeInterval, RiseSpeed)};
	Attacker->SetActorLocation(NewLocation);
}

void UCharge_FromSky::CharacterFall()
{
	UE_LOG(LogTemp, Warning, TEXT("Falling"));
	FVector CurrenLocation{Attacker->GetActorLocation()};
	if (CurrenLocation.Equals(LandingLocation, 10.0f))
	{
		Attacker->SetActorLocation(LandingLocation);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		RestartAI();
		return;
	}
	
	FVector NewLocation{FMath::VInterpTo(CurrenLocation, LandingLocation, TimeInterval, FallSpeed)};
	Attacker->SetActorLocation(NewLocation);
}

void UCharge_FromSky::RestartAI()
{
	UE_LOG(LogTemp, Warning, TEXT("Restarting AI"));
	AAIController* AIController{Cast<AAIController>(Attacker->GetController())};
	if (AIController)
	{
		AIController->BrainComponent->ResumeLogic("Finished charging from the sky");
	}
	Attacker->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}