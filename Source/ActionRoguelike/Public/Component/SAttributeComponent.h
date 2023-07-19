// Fill out your copyright notice in the Description page of Project Settings.
// 该类为玩家属性组件
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

// 注册生命值改变事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatordActor, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);

class USSingleAttribute;

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

	// 杀死自身
	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatordActor);

	// Sets default values for this component's properties
	USAttributeComponent();

	UPROPERTY(EditAnywhere, Category = "Attributes")
	TSubclassOf<USSingleAttribute> EnergyClass;
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	USSingleAttribute* Energy;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Attributes")
	float Health; // 生命值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float HealthMax; // 最大生命值

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatordActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRageChanged(AActor* InstigatordActor, float NewRage, float Delta);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Rage; // 怒气值

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float RageMax; // 最大怒气值

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
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth();

	// 最大生命值获取
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthMax();

	// 怒气获取
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRage() const;

	// 最大怒气获取
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRageMax() const;

	// 改变怒气
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRage(AActor* InstigatordActor, float Delta);

	virtual void BeginPlay();

	// 注册生命值改变事件
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	// 注册怒气值改变事件
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;
};
