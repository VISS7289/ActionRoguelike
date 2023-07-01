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

};
