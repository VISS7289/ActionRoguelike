// Fill out your copyright notice in the Description page of Project Settings.
// 这个类是传送弹
#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASDashProjectile();
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay; // 传送延时

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay; // 引爆延时

	// 引爆计时器
	FTimerHandle FTimerHandle_DetonateDelay;

	// 爆炸重载
	virtual void Explode_Implementation() override;

	// 实现传送
	void TeleportInstigator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
