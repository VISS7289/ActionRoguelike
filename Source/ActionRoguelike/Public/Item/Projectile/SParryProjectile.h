// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Projectile/SProjectileBase.h"
#include "GameplayTagContainer.h"
#include "SParryProjectile.generated.h"


class USActionEffect;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASParryProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ParryTag; // 伤害

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USActionEffect> BurningActionClass;

	// 组件碰撞事件处理函数
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
