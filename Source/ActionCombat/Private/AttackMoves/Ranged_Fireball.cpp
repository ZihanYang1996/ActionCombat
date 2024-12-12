// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackMoves/Ranged_Fireball.h"

#include "GameFramework/Character.h"

void URanged_Fireball::Setup(ACharacter* AttackingCharacter)
{
	Attacker = AttackingCharacter;
}

float URanged_Fireball::Execute()
{
	return Attacker->PlayAnimMontage(RangedAttack);
	
}

