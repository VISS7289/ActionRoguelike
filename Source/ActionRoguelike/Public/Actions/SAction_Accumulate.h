// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SActionWeapon.h"
#include "Tickable.h"
#include "TimerManager.h"
#include "Components/TimelineComponent.h"
#include "SAction_Accumulate.generated.h"

class USpringArmComponent; // 弹簧臂组件

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Accumulate : public USActionWeapon, public FTickableGameObject
{
	GENERATED_BODY()

public:

	USAction_Accumulate();

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

	// 相机聚焦相关
	float CameraAccumulate;
	FVector StartPos;
	FVector NowPos;
	FVector EndPos;
	UPROPERTY()
	USpringArmComponent* SpringArmComp; // 弹簧臂组件

	// 聚焦状态相关
	UPROPERTY(EditDefaultsOnly, Category = "Accumulate")
	float MaxAccumulateTime;
	float CurrentAccumulateTime;

	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	virtual void StopAction_Implementation(AActor* InstigatorActor) override;

	UFUNCTION()
	void SetupTimeline();
	UFUNCTION()
	void TimelineProgressFunction(float Value);
	
};
