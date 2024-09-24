// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BossCharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/StatsComponent.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComponent = CreateDefaultSubobject<UStatsComponent>("Stats Component");
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &ABossCharacter::DamageReceidved);
	
	BlackboardComponent = GetController<AAIController>()->GetBlackboardComponent();
	BlackboardComponent->SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(InitialState));
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

void ABossCharacter::DamageReceidved(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                                     AController* InstigatedBy, AActor* DamageCauser)
{
	StatsComponent->ReduceHealth(Damage);
}

void ABossCharacter::DetectPawn(APawn* PawnDetected, APawn* PawnToDetect) const
{
	// If the detected pawn is not the one to detect or the current state is already range, return
	EEnemyState CurrentState = static_cast<EEnemyState>(BlackboardComponent->GetValueAsEnum(TEXT("CurrentState")));
	if (PawnDetected != PawnToDetect || CurrentState == EEnemyState::Range)
	{
		return;
	}
	BlackboardComponent->SetValueAsEnum(TEXT("CurrentState"), static_cast<uint8>(EEnemyState::Range));
}
