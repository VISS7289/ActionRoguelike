// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SAction_Reload.generated.h"

class USWeaponComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Reload : public USAction
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	USWeaponComponent* WeaponComp;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* ReloadAnim; // ªªµØ√…Ã´∆Ê

	FTimerHandle TimerHandle_PrimaryAttack; // —” ±

	UFUNCTION()
	void ReloadBullet_Elapsed(ACharacter* InstigatorCharacter);

public:

	virtual bool CanStart_Implementation(AActor* InstigatorActor) override;

	virtual void StartAction_Implementation(AActor* InstigatorActor) override;


	
};
