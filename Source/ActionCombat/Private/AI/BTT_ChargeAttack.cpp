// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_ChargeAttack.h"

#include "AIController.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef{OwnerComp.GetAIOwner()->GetCharacter()};
	
	if (!IsValid(CharacterRef))
	{
		return EBTNodeResult::Failed;
	}

	// CharacterRef->PlayAnimMontage(ChargeAttack);
	UE_LOG(LogTemp, Warning, TEXT("Charge attack!"));
	return EBTNodeResult::Succeeded;
}
