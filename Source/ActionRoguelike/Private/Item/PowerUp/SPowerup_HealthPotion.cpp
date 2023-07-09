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
	// ��������ǿ�
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	// ��ȡ����������������
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	// ������������ҷ���Ѫ
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			// ��Ѫ�ɹ��ͽ�����ȴ
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPowerup();
			}
		}
		
	}

}