// Fill out your copyright notice in the Description page of Project Settings.
// 该类为玩家积分
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

// 注册积分改变事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);


class USSaveGame;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 Credits; // 积分

public:

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	// 积分增加
	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	// 积分减少
	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);

	UPROPERTY(BlueprintAssignable, Category = "Credits")
	FOnCreditsChanged OnCreditsChanged;
	
};
