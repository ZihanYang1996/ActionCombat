// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_ChargeAttack.h"

#include "AIController.h"
#include "Animations/BossAnimInstance.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIControllerPtr = OwnerComp.GetAIOwner();
	
	CharacterPtr = AIControllerPtr->GetCharacter();

	BossAnimInstancePtr = Cast<UBossAnimInstance>(CharacterPtr->GetMesh()->GetAnimInstance());

	BossAnimInstancePtr->bIsCharging = true;
	return EBTNodeResult::InProgress;
}
