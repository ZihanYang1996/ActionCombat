// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackMoves/Ranged_Fireball.h"

#include "GameFramework/Character.h"

void URanged_Fireball::Setup()
{
	
}

void URanged_Fireball::Execute(ACharacter* Character, float& Duration) const
{
	Character->PlayAnimMontage(RangedAttack);	
}

