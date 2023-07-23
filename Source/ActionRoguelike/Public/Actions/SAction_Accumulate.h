// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SActionWeapon.h"
#include "Tickable.h"
#include "TimerManager.h"
#include "Components/TimelineComponent.h"
#include "SAction_Accumulate.generated.h"

class USpringArmComponent; // ���ɱ����

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Accumulate : public USActionWeapon, public FTickableGameObject
{
	GENERATED_BODY()

public:

	USAction_Accumulate();
	virtual void Initialize_Implementation(USActionComponent* NewActionComp) override;
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	virtual void StopAction_Implementation(AActor* InstigatorActor) override;

public:
	// ������FTickableGameObject�������صĺ���
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(MyBPObject, STATGROUP_Tickables);
	}
	virtual bool IsTickable() const override { return !this->IsDefaultSubobject(); }
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Accumulate")
	UAnimMontage* AccuStartAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Accumulate")
	UAnimMontage* AccuLoopAnim;

protected:

	UFUNCTION()
	void PlayAccuLoop(UAnimMontage* Montage, bool bInterrupted);

protected:

	// Timeline���
	bool HasInit;
	FTimeline CurveTimeline;
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	UCurveFloat* Curve;

	// ����۽����
	float CameraAccumulate;
	FVector StartPos;
	FVector NowPos;
	FVector EndPos;
	UPROPERTY()
	USpringArmComponent* SpringArmComp; // ���ɱ����
	UPROPERTY()
	ACharacter* AccuCharacter;

	// �۽�״̬���
	UPROPERTY(EditDefaultsOnly, Category = "Accumulate")
	float MaxAccumulateTime;
	float CurrentAccumulateTime;

	UFUNCTION()
	void SetupTimeline();
	UFUNCTION()
	void TimelineProgressFunction(float Value);
	
};
