// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SActionWeapon.generated.h"

class USWeaponComponent;


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionWeapon : public USAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(Replicated)
	USWeaponComponent* WeaponComp;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName HandSocketName; // ����������������

	FTimerHandle TimerHandle_PrimaryAttack; // ��ʱ

	UFUNCTION()
	virtual void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

protected:

	UPROPERTY(Replicated)
	UAnimInstance* CharacterAnimIns;
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim; // ��ͨ������̫��
	UPROPERTY(Replicated)
	ACharacter* FireCharacter;

protected:

	// ʵʩ����
	virtual void PlayFireAnim();
	UFUNCTION()
	virtual void FireNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
	virtual void FireAnimEnd(UAnimMontage* Montage, bool bInterrupted);

public:

	virtual void Initialize_Implementation(USActionComponent* NewActionComp) override;
	virtual bool CanStart_Implementation(AActor* InstigatorActor) override;
	virtual void StopAction_Implementation(AActor* InstigatorActor) override;

	USActionWeapon();
	
};
