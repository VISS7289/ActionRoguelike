// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "Tickable.h"
#include "TimerManager.h"
#include "Components/TimelineComponent.h"
#include "SAction_Dash.generated.h"

class UTimelineComponent;
class ASCharacter;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Dash : public USAction, public FTickableGameObject
{
	GENERATED_BODY()

public:

	USAction_Dash();

	virtual void Initialize(USActionComponent* NewActionComp);
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

public:
	// 抽象类FTickableGameObject必须重载的函数
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(MyBPObject, STATGROUP_Tickables);
	}
	virtual bool IsTickable() const override { return !this->IsDefaultSubobject(); }
	virtual void Tick(float DeltaTime) override;

protected:

	// Timeline相关
	bool HasInit;
	FTimeline CurveTimeline;
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	UCurveFloat* Curve;

	UFUNCTION()
	void SetupTimeline();
	UFUNCTION()
	void TimelineProgressFunction(float Value);
	UFUNCTION()
	void TimelineCallbackFunction();

protected:

	UPROPERTY()
	UAnimInstance* DashAnimIns;
	UPROPERTY()
	UAnimMontage* SelectAnim;
	UPROPERTY(EditDefaultsOnly, Category = "DashAnim")
	UAnimMontage* FwdDashAnim;
	UPROPERTY(EditDefaultsOnly, Category = "DashAnim")
	UAnimMontage* BackDashAnim;
	UPROPERTY(EditDefaultsOnly, Category = "DashAnim")
	UAnimMontage* LeftDashAnim;
	UPROPERTY(EditDefaultsOnly, Category = "DashAnim")
	UAnimMontage* RightDashAnim;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	float DashLength; // 冲刺距离
	UPROPERTY()
	FVector StartPos;
	UPROPERTY()
	FVector NowPos;
	UPROPERTY()
	FVector EndPos;

	UPROPERTY()
	ASCharacter* DashSCharacter;
	UPROPERTY()
	UCameraComponent* DashCamera;

	float GetYawByVector(FVector Fwd, FVector Act);

	void SelectAnimByVector(FVector Fwd, FVector Act);

	FVector GetForward();
	
};
