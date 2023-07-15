// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SAction_Fire.generated.h"

class USWeaponComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Fire : public USAction
{
	GENERATED_BODY()
	
protected:

	UPROPERTY()
	USWeaponComponent* WeaponComp;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim; // 普通攻击蒙太奇

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName HandSocketName; // 发射抛射物插槽名称

	FTimerHandle TimerHandle_PrimaryAttack; // 延时

	// 实施攻击
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:

	// 开始远程攻击行动
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	USAction_Fire();

};
