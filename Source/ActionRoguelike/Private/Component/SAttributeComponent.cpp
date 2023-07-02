// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SAttributeComponent.h"
#include "SGameModeBase.h"

// 伤害倍乘作弊代码
static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier For Attribute Component."), ECVF_Cheat);

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
// 根据变化量Delta与作弊信息CVarDamageMultiplier更新生命值，并对死亡进行处理。在处理时会广播OnHealthChanged。
bool USAttributeComponent::ApplyHealthChange(AActor* InstigatordActor, float Delta)
{

	if (Delta < 0.0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	float OldHealth = Health; // 过去生命值
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float ActualDelta = Health - OldHealth; // 实际变化量
	OnHealthChanged.Broadcast(InstigatordActor, this, Health, ActualDelta);

	if (Delta < 0.0f && Health <= 0.0f)
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatordActor);
		}
		
	}

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