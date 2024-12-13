// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackMoves/Ranged_Fireball.h"

#include "Combat/ContinueAttackNotify.h"
#include "GameFramework/Character.h"

void URanged_Fireball::Setup(ACharacter* AttackingCharacter)
{
	Attacker = AttackingCharacter;

	const TArray<FAnimNotifyEvent>& NotifyEvents{PreAttack->Notifies};
	for (const FAnimNotifyEvent& NotifyEvent : NotifyEvents)
	{
		if (UContinueAttackNotify* Notify{Cast<UContinueAttackNotify>(NotifyEvent.Notify)})
		{
			Notify->SetNetMontage(RangedAttack);
		}
		TotalDuration = NotifyEvent.GetTriggerTime();  // Get the time length before the attack is triggered
	}

	TotalDuration += RangedAttack->GetPlayLength();
}

float URanged_Fireball::Execute()
{
	Attacker->PlayAnimMontage(PreAttack);
	return TotalDuration;
}

