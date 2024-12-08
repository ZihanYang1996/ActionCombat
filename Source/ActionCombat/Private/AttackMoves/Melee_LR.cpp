// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackMoves/Melee_LR.h"

#include "GameFramework/Character.h"

void UMelee_LR::Execute(ACharacter* Character, float& Duration) const
{
	Duration = Character->PlayAnimMontage(Melee_L_Montage);
}
