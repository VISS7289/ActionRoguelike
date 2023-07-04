// Fill out your copyright notice in the Description page of Project Settings.
// ����ΪBUFF
#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:

	// BUFF����
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;
	// BUFF����
	virtual void StopAction_Implementation(AActor* InstigatorActor) override;

	USActionEffect();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration; // ����ʱ��

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period; // ������Ч��

	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;
	
	UFUNCTION(BlueprintNativeEvent)
	void ExecutePeriodicEffect(AActor* InstigatorActor); // BUFF����Ч��ִ��
};
