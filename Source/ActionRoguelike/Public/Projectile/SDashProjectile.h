// Fill out your copyright notice in the Description page of Project Settings.
// ������Ǵ��͵�
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
	float TeleportDelay; // ������ʱ

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay; // ������ʱ

	// ������ʱ��
	FTimerHandle FTimerHandle_DetonateDelay;

	// ��ը����
	virtual void Explode_Implementation() override;

	// ʵ�ִ���
	void TeleportInstigator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
