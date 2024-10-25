// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"

#include "Animations/PlayerAnimInstance.h"
#include "Characters/StatsComponent.h"
#include "Characters/ECharacterStat.h"
#include "Characters/PlayerActionsComponent.h"
#include "Combat/BlockComponent.h"
#include "Combat/CombatComponent.h"
#include "Combat/LockonComponent.h"
#include "Combat/TraceComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));
	LockonComponent = CreateDefaultSubobject<ULockonComponent>(TEXT("Lockon Component"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	TraceComponent = CreateDefaultSubobject<UTraceComponent>(TEXT("Trace Component"));
	BlockComponent = CreateDefaultSubobject<UBlockComponent>(TEXT("Block Component"));
	PlayerActionsComponent = CreateDefaultSubobject<UPlayerActionsComponent>(TEXT("Player Actions Component"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get Animation Instance
	PlayerAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	OnTakeAnyDamage.AddDynamic(this, &AMainCharacter::OnDamageReceived);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StatsComponent->RegenerateStamina();

	double CurrentVelocity{GetVelocity().Length()};
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,
	                                 FString::Printf(TEXT("Current Velocity: %f"), CurrentVelocity));
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AMainCharacter::GetDamage()
{
	return StatsComponent->Stats[ECharacterStat::Strength];
}

bool AMainCharacter::HasEnoughStamina_Implementation(float StaminaCost)
{
	return StatsComponent->Stats[ECharacterStat::Stamina] >= StaminaCost;
}

void AMainCharacter::OnDamageReceived(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                      AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Damage Causer: %s"), *DamageCauser->GetName());
	if (CanTakeDamage(DamageCauser))
	{
		StatsComponent->ReduceHealth(Damage);
		PlayAnimMontage(HurtAnimMontage);
	}
}

void AMainCharacter::HandleDeath()
{
	PlayAnimMontage(DeathAnimMontage);
	DisableInput(Cast<APlayerController>(GetController()));
}

void AMainCharacter::AbortLockon(AActor* TargetActor)
{
	LockonComponent->AbortLockon(TargetActor);
}

void AMainCharacter::StartBlock()
{
	PlayerAnimInstance->bIsBlocking = true;
}

void AMainCharacter::EndBlock()
{
	PlayerAnimInstance->bIsBlocking = false;
}

bool AMainCharacter::CanTakeDamage(AActor* DamageCauser) const
{
	if (PlayerActionsComponent->bIsInvincible)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Attack dodged"));
		return false;
	}
	if (PlayerAnimInstance->bIsBlocking)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Attack blocked"));
		return !BlockComponent->CanBlock(DamageCauser);
	}
	return true;
}

bool AMainCharacter::CanTakeInput()
{
	if (!CombatComponent->CanTakeINput())
	{
		return false;
	}

	return true;
}
