// Fill out your copyright notice in the Description page of Project Settings.
// 该类为BUFF
#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:

	// BUFF启动
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	// BUFF结束
	virtual void StopAction_Implementation(AActor* InstigatorActor) override;

	USActionEffect();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration; // 持续时间

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period; // 周期性效果

	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;
	
	UFUNCTION(BlueprintNativeEvent)
	void ExecutePeriodicEffect(AActor* InstigatorActor); // BUFF周期效果执行
};
