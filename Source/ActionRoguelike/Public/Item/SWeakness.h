// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeakness.generated.h"

class USAttributeComponent; // 玩家属性组件
class UStaticMeshComponent; // 静态网格体

UCLASS()
class ACTIONROGUELIKE_API ASWeakness : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeakness();

	void SetAttrComp(USAttributeComponent* AttrComp);

protected:
	
	UPROPERTY(VisibleAnywhere)
	USAttributeComponent* AttributeComp; // 弱点属性组件

	UPROPERTY()
	USAttributeComponent* OwnerAttributeComp; // 弱点属性组件

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp; // 静态网格体

	UPROPERTY(EditDefaultsOnly)
	float DamageMultiply;

	// 弱点伤害处理
	UFUNCTION()
	void GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
