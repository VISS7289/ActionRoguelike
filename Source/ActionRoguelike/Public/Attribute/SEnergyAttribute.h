// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attribute/SSingleAttribute.h"
#include "SEnergyAttribute.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USEnergyAttribute : public USSingleAttribute
{
	GENERATED_BODY()

public:

	// 注册精力值改变事件
	UPROPERTY(BlueprintAssignable)
	FOnAttributeValueChanged OnEnergyChanged;

protected:

	virtual void ApplyBroadCast(AActor* InstigatordActor, float NewValue, float Delta);
	
};
