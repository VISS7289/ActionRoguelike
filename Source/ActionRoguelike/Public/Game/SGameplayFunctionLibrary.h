// Fill out your copyright notice in the Description page of Project Settings.
// 该类为函数库
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// 造成伤害函数
	UFUNCTION(BlueprintCallable)
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount); 

	// 有方向的伤害函数
	UFUNCTION(BlueprintCallable)
	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);

	// 造成怒气函数
	UFUNCTION(BlueprintCallable)
	static bool ApplyRage(AActor* RageCauser, AActor* TargetActor, float RageAmount);

	// 造成精力变化
	UFUNCTION(BlueprintCallable)
	static bool ApplyEnergy(AActor* EnergyCauser, AActor* TargetActor, float EnergyAmount);

	// 多人状态下的日志打印
	UFUNCTION(BlueprintCallable)
	static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.0f);

};
