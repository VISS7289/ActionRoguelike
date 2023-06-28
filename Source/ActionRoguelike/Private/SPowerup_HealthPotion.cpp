// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"

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
		// ��Ѫ�ɹ��ͽ�����ȴ
		if (AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax()))
		{
			HideAndCooldownPowerup();
		}
	}

}