// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SAction_Jump.generated.h"

class ACharacter;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Jump : public USAction
{
	GENERATED_BODY()

public:

	virtual void Initialize(USActionComponent* NewActionComp);
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	virtual void StopAction_Implementation(AActor* InstigatorActor) override;

protected:

	UPROPERTY()
	ACharacter* OwningCharacter;
	
};
