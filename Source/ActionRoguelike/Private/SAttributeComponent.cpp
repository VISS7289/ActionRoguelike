// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
	HealthMax = 100;
}

// 杀死自身
bool USAttributeComponent::Kill(AActor* InstigatordActor)
{
	return ApplyHealthChange(InstigatordActor, -GetHealthMax());
}

// 生命值改变
bool USAttributeComponent::ApplyHealthChange(AActor* InstigatordActor, float Delta)
{
	// 记录过去生命并加血
	float OldHealth = Health; 
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	// 计算变化量
	float ActualDelta = Health - OldHealth;
	// 触发生命值改变事件
	OnHealthChanged.Broadcast(InstigatordActor, this, Health, ActualDelta);
	// 返回结果
	return ActualDelta != 0;
}

// 访问生命值
float USAttributeComponent::GetHealth()
{
	return Health;
}

// 最大生命值获取
float USAttributeComponent::GetHealthMax()
{
	return HealthMax;
}

// 是否活着
bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

// 是否满血
bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

// 返回Actor上的USAttributeComponent
USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

// 判断Actor是否活着
bool USAttributeComponent::IsActorAlive(AActor* FromActor)
{
	USAttributeComponent* AttributeComp = GetAttributes(FromActor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}