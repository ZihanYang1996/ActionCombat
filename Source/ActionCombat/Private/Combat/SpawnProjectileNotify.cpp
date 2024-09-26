// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/SpawnProjectileNotify.h"

#include "Combat/EnemyProjectileComponent.h"

void USpawnProjectileNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (UEnemyProjectileComponent* ProjectileComp = MeshComp->GetOwner()->GetComponentByClass<UEnemyProjectileComponent>())
	{
		ProjectileComp->SpawnProjectile();
	}
}
