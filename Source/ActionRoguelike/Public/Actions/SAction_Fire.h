// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SActionWeapon.h"
#include "SAction_Fire.generated.h"

class USWeaponComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Fire : public USActionWeapon
{
	GENERATED_BODY()
	
public:

	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	virtual void StopAction_Implementation(AActor* InstigatorActor) override;
	virtual void FireAnimEnd(UAnimMontage* Montage, bool bInterrupted);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool isLoop;

	bool FireEnd;


};
