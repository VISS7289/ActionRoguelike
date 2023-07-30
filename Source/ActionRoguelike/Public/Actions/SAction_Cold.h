// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction_Cold.generated.h"

class UWorld;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Cold : public UObject
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, Category = "Cold")
	float ClodTime;
	bool InCold;
	FTimerHandle FTimerHandle_ColdDelay;

	UWorld* ColdWorld;

public:

	USAction_Cold();

	void InitCold(float NewCloadTime);

	UFUNCTION(BlueprintCallable, Category = "Cold")
	void StartCold();

	UFUNCTION(BlueprintCallable, Category = "Cold")
	bool IsInCold();

	UFUNCTION()
	void ColdFinish();

	void SetCloadTime(float NewCloadTime);

	float GetCloadTime();
	
};
