// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
	HealthMax = 100;
}


// ����ֵ�ı�
bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	// ��¼��ȥ��������Ѫ
	float OldHealth = Health; 
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	// ����仯��
	float ActualDelta = Health - OldHealth;
	// ��������ֵ�ı��¼�
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);
	// ���ؽ��
	return ActualDelta != 0;
}

// ��������ֵ
float USAttributeComponent::GetHealth()
{
	return Health;
}

// �������ֵ��ȡ
float USAttributeComponent::GetHealthMax()
{
	return HealthMax;
}

// �Ƿ����
bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

// �Ƿ���Ѫ
bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}