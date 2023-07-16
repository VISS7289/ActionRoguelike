// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_BulletChange.h"

bool USAction_BulletChange::CanStart_Implementation(AActor* InstigatorActor)
{
	Super::CanStart_Implementation(InstigatorActor);

	return true;
}

void USAction_BulletChange::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::CanStart_Implementation(InstigatorActor);

}
