// Fill out your copyright notice in the Description page of Project Settings.
// 该类为黑洞子弹
#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SBlackHoleProjectile.generated.h"


class URadialForceComponent; // 径向力组件
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASBlackHoleProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASBlackHoleProjectile();

protected:

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp; // 径向力组件

	// 注册事件回调函数
	virtual void PostInitializeComponents() override;

	// 组件碰撞事件处理函数  
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
