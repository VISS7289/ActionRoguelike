// Fill out your copyright notice in the Description page of Project Settings.
// 该类是炸药桶
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class UStaticMeshComponent; // 静态网格体
class URadialForceComponent; // 径向力组件


UCLASS()
class ACTIONROGUELIKE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp; // 静态网格体

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp; // 径向力组件

	// 注册事件回调函数
	virtual void PostInitializeComponents() override;

	UFUNCTION(NetMulticast, UnReliable)
	void MulticastExplode();

	// 组件碰撞事件处理函数  
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
