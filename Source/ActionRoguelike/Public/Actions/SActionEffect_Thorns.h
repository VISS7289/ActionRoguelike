// Fill out your copyright notice in the Description page of Project Settings.
// 该类为反伤甲
#pragma once

#include "CoreMinimal.h"
#include "Actions/SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"


class USAttributeComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;  // 反伤百分比

	// 生命值改变时反伤
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

public:

	// Action开始时，监听生命值改变事件
	virtual void StartAction_Implementation(AActor* Instigator) override;

	// Action结束时，取消监听生命值改变事件
	virtual void StopAction_Implementation(AActor* Instigator) override;

	USActionEffect_Thorns();


};
