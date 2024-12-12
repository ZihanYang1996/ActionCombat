// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackMoves/AttackMove.h"

UAttackMove::~UAttackMove()
{
	UE_LOG(LogTemp, Error, TEXT("Attack Move Destroyed"));
}
