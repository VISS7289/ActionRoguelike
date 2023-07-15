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
	UAnimMontage* AttackAnim; // ��ͨ������̫��

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName HandSocketName; // ����������������

	FTimerHandle TimerHandle_PrimaryAttack; // ��ʱ

	// ʵʩ����
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:

	// ��ʼԶ�̹����ж�
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

	USAction_Fire();

};
