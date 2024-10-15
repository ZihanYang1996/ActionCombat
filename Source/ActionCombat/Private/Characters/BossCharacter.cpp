// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BossCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/StatsComponent.h"
#include "Combat/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/MainPlayer.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	AIController = GetController<AAIController>();

	OnTakeAnyDamage.AddDynamic(this, &ABossCharacter::DamageReceived);

	BlackboardComponent = AIController->GetBlackboardComponent();
	BlackboardComponent->SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(InitialState));

	if (APawn* PlayerPawnPtr{GetWorld()->GetFirstPlayerController()->GetPawn()})
	{
		PlayerPawnPtr->GetComponentByClass<UStatsComponent>()->OnZeroHealthDelegate
		             .AddDynamic(this, &ABossCharacter::HandlePlayerDeath);
	}
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// void ABossCharacter::OnSelected_Implementation()
// {
// 	// Do something when the boss is selected
// 	UE_LOG(LogTemp, Warning, TEXT("Blueprint implementation is forgotten!"));
// }

void ABossCharacter::DamageReceived(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                    AController* InstigatedBy, AActor* DamageCauser)
{
	StatsComponent->ReduceHealth(Damage);
}

void ABossCharacter::DetectPawn(APawn* PawnDetected, APawn* PawnToDetect) const
{
	// If the detected pawn is not the one to detect or the current state is already range, return
	EEnemyState CurrentState = static_cast<EEnemyState>(BlackboardComponent->GetValueAsEnum(TEXT("CurrentState")));
	if (PawnDetected != PawnToDetect || CurrentState != EEnemyState::Idle)
	{
		return;
	}
	BlackboardComponent->SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(EEnemyState::Range));
}

float ABossCharacter::GetDamage()
{
	return StatsComponent->Stats[ECharacterStat::Strength];
}

void ABossCharacter::Attack()
{
	CombatComponent->RandomAttack();
}

float ABossCharacter::GetAnimDuration() const
{
	return CombatComponent->AnimDuration;
}

float ABossCharacter::GetMeleeRange() const
{
	return StatsComponent->Stats[ECharacterStat::MeleeRange];
}

void ABossCharacter::HandlePlayerDeath()
{
	BlackboardComponent->SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(EEnemyState::GameOver));
}

void ABossCharacter::HandleDeath()
{
	float DeathAnimationLength{PlayAnimMontage(DeathAnimMontage)};
	AIController->BrainComponent->StopLogic("Defeated");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (IMainPlayer* MainPlayerInterfacePtr{Cast<IMainPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())})
	{
		MainPlayerInterfacePtr->AbortLockon();
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABossCharacter::AfterDeathAnimation,
	                                       DeathAnimationLength + 2, false);
}

void ABossCharacter::AfterDeathAnimation()
{
	Destroy();
}
