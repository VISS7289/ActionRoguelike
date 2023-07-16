// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SAction_BulletChange.generated.h"

class USWeaponComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_BulletChange : public USAction
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	USWeaponComponent* WeaponComp;

public:

	virtual bool CanStart_Implementation(AActor* InstigatorActor) override;
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	
};
