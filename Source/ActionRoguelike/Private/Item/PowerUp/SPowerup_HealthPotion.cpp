// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PowerUp/SPowerup_HealthPotion.h"
#include "Component/SAttributeComponent.h"
#include "Player/SPlayerState.h"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	CreditCost = 50;
}

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
		if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			// 加血成功就进入冷却
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPowerup();
			}
		}
		
	}

}