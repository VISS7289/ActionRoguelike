// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ������
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

	// ����˺�����
	UFUNCTION(BlueprintCallable)
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount); 

	// �з�����˺�����
	UFUNCTION(BlueprintCallable)
	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);

	// ���ŭ������
	UFUNCTION(BlueprintCallable)
	static bool ApplyRage(AActor* RageCauser, AActor* TargetActor, float RageAmount);

};
