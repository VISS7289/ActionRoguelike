// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SAction_Flash.generated.h"

class UCameraComponent; // ÉãÏñ»ú×é¼þ

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Flash : public USAction
{
	GENERATED_BODY()

public:

	USAction_Flash();

	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	float FlashLength; // ³å´Ì¾àÀë

	UPROPERTY()
	AActor* FlashActor;
	
	UPROPERTY()
	UCameraComponent* CameraComp;

	FCollisionShape CollShap;

	FVector CollisionCheck(AActor* InstigatorActor, FVector StartPos, FVector EndPos);
};
