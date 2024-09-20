// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/StatsComponent.h"

#include "Combat/CombatComponent.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UCombatComponent* CombatComponent = GetOwner()->FindComponentByClass<UCombatComponent>())
	{
		CombatComponent->OnAttackPerformedDelegate.AddDynamic(this, &UStatsComponent::ReduceStamina);
	}
}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatsComponent::ReduceHealth(float Amount)
{
	Stats[ECharacterStat::Health] = FMath::Clamp((Stats[ECharacterStat::Health] - Amount), 0.0f,
	                                             Stats[ECharacterStat::MaxHealth]);
	UE_LOG(LogTemp, Warning, TEXT("Received %f damage! Remaining health is %f"), Amount, Stats[ECharacterStat::Health]);
}

void UStatsComponent::ReduceStamina(float Amount)
{
	Stats[ECharacterStat::Stamina] = FMath::Clamp((Stats[ECharacterStat::Stamina] - Amount), 0.0f,
	                                              Stats[ECharacterStat::MaxStamina]);
	UE_LOG(LogTemp, Warning, TEXT("Stamina reduced to %f"), Stats[ECharacterStat::Stamina]);
}
