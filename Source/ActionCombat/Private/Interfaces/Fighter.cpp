// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/Fighter.h"

// Add default functionality here for any IFighter functions that are not pure virtual.

float IFighter::GetDamage()
{
	return 0.0f;
}

void IFighter::Attack()
{
}

float IFighter::GetAnimDuration() const
{
	return 0.0f;
}

float IFighter::GetMeleeRange() const
{
	return 0.0f;
}

bool IFighter::CanTakeDamage(AActor* DamageCauser) const
{
	return true;
}


