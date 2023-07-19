// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/SEnergyAttribute.h"

void USEnergyAttribute::ApplyBroadCast(AActor* InstigatordActor, float NewValue, float Delta)
{
	OnEnergyChanged.Broadcast(InstigatordActor, OwnerComp, NewValue, Delta);
}