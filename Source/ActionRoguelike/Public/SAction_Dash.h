// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "Tickable.h"
#include "TimerManager.h"
#include "Components/TimelineComponent.h"
#include "SAction_Dash.generated.h"

class UTimelineComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Dash : public USAction, public FTickableGameObject
{
	GENERATED_BODY()

public:

	USAction_Dash();

	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

public:
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(MyBPObject, STATGROUP_Tickables);
	}
	virtual bool IsTickable() const override { return !this->IsDefaultSubobject(); }
	virtual void Tick(float DeltaTime) override;

protected:

	bool HasInit;

	FTimeline CurveTimeline;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	UCurveFloat* Curve;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	float DashLength;

	UPROPERTY()
	FVector StartPos;
	UPROPERTY()
	FVector NowPos;
	UPROPERTY()
	FVector EndPos;

	UPROPERTY()
	AActor* DashActor;

	UFUNCTION()
	void SetupTimeline();

	UFUNCTION()
	void TimelineProgressFunction(float Value);

	UFUNCTION()
	void TimelineCallbackFunction();

	
};
