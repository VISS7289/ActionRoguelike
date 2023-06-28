// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	// 交互对象非空
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	// 获取交互对象的属性组件
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	// 存在属性组件且非满血
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		// 加血成功就进入冷却
		if (AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax()))
		{
			HideAndCooldownPowerup();
		}
	}

}