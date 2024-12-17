// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackMoves/Melee_LR.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/ContinueAttackNotify.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"

float UMelee_LR::Execute()
{
	AAIController* AIController{Attacker->GetController<AAIController>()};
	if (!IsValid(AIController))
	{
		UE_LOG(LogTemp, Error, TEXT("AIController is nullptr!"));
		return 0.0f;		
	}
	
	float DistanceToPlayer{AIController->GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceToPlayer"))};
	float AttackRadius{AIController->GetBlackboardComponent()->GetValueAsFloat(TEXT("MeleeAttackRadius"))};
	if (DistanceToPlayer > AttackRadius)
	{
		return Attacker->PlayAnimMontage(PreAttack);
	}
	else if (DistanceToPlayer != 0.0f)
	{
		return Attacker->PlayAnimMontage(Attack);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DistanceToPlayer is 0!"));
		return 0.0f;
	}
}

void UMelee_LR::Setup(ACharacter* AttackingCharacter)
{
	Attacker = AttackingCharacter;
}