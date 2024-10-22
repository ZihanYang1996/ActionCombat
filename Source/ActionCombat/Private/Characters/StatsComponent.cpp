// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/StatsComponent.h"

#include "Characters/PlayerActionsComponent.h"
#include "Combat/BlockComponent.h"
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

	if (UPlayerActionsComponent* PlayerActionsComponent = GetOwner()->FindComponentByClass<UPlayerActionsComponent>())
	{
		PlayerActionsComponent->OnSprintDelegate.AddDynamic(this, &UStatsComponent::ReduceStamina);
	}

	if (UBlockComponent* BlockComponent = GetOwner()->GetComponentByClass<UBlockComponent>())
	{
		BlockComponent->OnBlockDelegate.AddDynamic(this, &UStatsComponent::ReduceStamina);
	}
}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatsComponent::RegenerateStamina()
{
	if (!bCanStaminaRegen)
	{
		return;
	}
	Stats[ECharacterStat::Stamina] = FMath::FInterpConstantTo(Stats[ECharacterStat::Stamina],
	                                                          Stats[ECharacterStat::MaxStamina],
	                                                          GetWorld()->GetDeltaSeconds(),
	                                                          StaminaRegenRate);
}

float UStatsComponent::GetStatPercentage(ECharacterStat Stat, ECharacterStat MaxStat) const
{
	return Stats[Stat] / Stats[MaxStat];
}

void UStatsComponent::ReduceHealth(float Amount)
{
	Stats[ECharacterStat::Health] = FMath::Clamp((Stats[ECharacterStat::Health] - Amount), 0.0f,
	                                             Stats[ECharacterStat::MaxHealth]);
	
	if (Stats[ECharacterStat::Health] == 0.0f)
	{
		OnZeroHealthDelegate.Broadcast();
	}
}

void UStatsComponent::ReduceStamina(float Amount)
{
	Stats[ECharacterStat::Stamina] = FMath::Clamp((Stats[ECharacterStat::Stamina] - Amount), 0.0f,
	                                              Stats[ECharacterStat::MaxStamina]);
	UE_LOG(LogTemp, Warning, TEXT("Stamina reduced to %f"), Stats[ECharacterStat::Stamina]);

	bCanStaminaRegen = false;
	
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimer, this, &UStatsComponent::EnableStaminaRegen,
	                                       StaminaDelayDuration, false);
}

void UStatsComponent::EnableStaminaRegen()
{
	bCanStaminaRegen = true;
}
