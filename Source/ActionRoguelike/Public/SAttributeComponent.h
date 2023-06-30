// Fill out your copyright notice in the Description page of Project Settings.
// 该类为玩家属性组件
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

// 注册生命值改变事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatordActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// 返回Actor上的USAttributeComponent
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	// 判断Actor是否活着
	UFUNCTION(BlueprintCallable, Category = "Attributes", meta=(DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* FromActor);

	// Sets default values for this component's properties
	USAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Health; // 生命值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float HealthMax; // 最大生命值


public:	

	// 是否活着
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	// 是否满血
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth() const;

	// 生命值改变
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatordActor, float Delta);

	// 生命值获取
	float GetHealth();

	// 最大生命值获取
	float GetHealthMax();

	// 注册生命值改变事件
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
};
