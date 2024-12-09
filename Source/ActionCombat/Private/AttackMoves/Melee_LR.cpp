// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackMoves/Melee_LR.h"

#include "Animations/ContinueAttackNotify.h"
#include "GameFramework/Character.h"

float UMelee_LR::Execute()
{
	Attacker->PlayAnimMontage(PreAttack);
	return TotalDuration;
}

void UMelee_LR::Setup(ACharacter* AttackingCharacter)
{
	Attacker = AttackingCharacter;
	const TArray<FAnimNotifyEvent>& NotifyEvents{PreAttack->Notifies};
	for (const FAnimNotifyEvent& NotifyEvent : NotifyEvents)
	{
		if (UContinueAttackNotify* Notify{Cast<UContinueAttackNotify>(NotifyEvent.Notify)})
		{
			Notify->SetNetMontage(Attack);
		}
	}

	TotalDuration = PreAttack->GetPlayLength() + Attack->GetPlayLength();
}