// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
}


// 生命值改变
bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	// 触发生命值改变事件
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

// 访问生命值
float USAttributeComponent::GetHealth()
{
	return Health;
}