// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_BulletChange.h"
#include "Component/SWeaponComponent.h"

bool USAction_BulletChange::CanStart_Implementation(AActor* InstigatorActor)
{
	if (!Super::CanStart_Implementation(InstigatorActor))
	{
		return false;
	}

	if (!WeaponComp)
	{
		WeaponComp = USWeaponComponent::GetWeaponComp(InstigatorActor);
	}

	return true;
}

void USAction_BulletChange::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::CanStart_Implementation(InstigatorActor);

	if (ChangeDirection == 1)
	{
		WeaponComp->BulletTypeRight();
	}
	else if (ChangeDirection == -1)
	{
		WeaponComp->BulletTypeLeft();
	}

	StopAction(InstigatorActor);

}
