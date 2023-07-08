// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

// 注册玩家改变事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	virtual void SetPawn(APawn* InPawn) override;
	
};
