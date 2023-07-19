// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SSingleAttribute.generated.h"

class USAttributeComponent;

// 注册属性值改变事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeValueChanged, AActor*, InstigatordActor, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USSingleAttribute : public UObject
{
	GENERATED_BODY()

public:

	USSingleAttribute();

	void InitValue(USAttributeComponent* AttributeComp);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Value; // 值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float ValueMax; // 最大值

	UPROPERTY()
	USAttributeComponent* OwnerComp;

	UFUNCTION()
	virtual void ApplyBroadCast(AActor* InstigatordActor, float NewValue, float Delta);

public:

	// 是否大于零
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsGreaterZero() const;

	// 是否最大值
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsValueMax() const;

	// 值改变
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyValueChange(AActor* InstigatordActor, float Delta);
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float CanValueChange(AActor* InstigatordActor, float Delta);

	// 值获取
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetValue();

	// 最大值获取
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetValueMax();

	// 注册值改变事件
	UPROPERTY(BlueprintAssignable)
	FOnAttributeValueChanged ValueChanged;
	
};
