// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/SSingleAttribute.h"
#include "Component/SAttributeComponent.h"

USSingleAttribute::USSingleAttribute()
{
	Value = 100.0f;
	ValueMax = 100.0f;
}

void USSingleAttribute::InitValue(USAttributeComponent* AttributeComp)
{
	OwnerComp = AttributeComp;
}

bool USSingleAttribute::IsGreaterZero() const
{
	return Value > 0;
}

bool USSingleAttribute::IsValueMax() const
{
	return Value == ValueMax;
}

bool USSingleAttribute::ApplyValueChange(AActor* InstigatordActor, float Delta)
{

	float OldValue = Value; // 过去值
	float NewValue = FMath::Clamp(Value + Delta, 0.0f, ValueMax);
	float ActualDelta = NewValue - OldValue; // 实际变化量

	Value = NewValue;
	if (ActualDelta != 0)
	{
		ApplyBroadCast(InstigatordActor, NewValue, Delta);
	}

	return ActualDelta != 0;
}

void USSingleAttribute::ApplyBroadCast(AActor* InstigatordActor, float NewValue, float Delta)
{
	ValueChanged.Broadcast(InstigatordActor, OwnerComp, NewValue, Delta);
}

float USSingleAttribute::CanValueChange(AActor* InstigatordActor, float Delta)
{
	float OldValue = Value; // 过去值
	float NewValue = FMath::Clamp(Value + Delta, 0.0f, ValueMax);
	float ActualDelta = NewValue - OldValue; // 实际变化量

	return ActualDelta;
}

float USSingleAttribute::GetValue()
{
	return Value;
}

float USSingleAttribute::GetValueMax()
{
	return ValueMax;
}