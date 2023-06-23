// Fill out your copyright notice in the Description page of Project Settings.
// 该类为魔法攻击抛射物
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class USphereComponent; // 球
class UProjectileMovementComponent; // 投射物移动组件
class UParticleSystemComponent; // 粒子系统

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp; // 球

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComp; // 投射物移动组件

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EffectComp; // 粒子系统

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
