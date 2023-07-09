// Fill out your copyright notice in the Description page of Project Settings.
// 这个类是击中闪烁材质
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STargetDummy.generated.h"

class USAttributeComponent; // 玩家属性组件
class UStaticMeshComponent; // 静态网格体

UCLASS()
class ACTIONROGUELIKE_API ASTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere)
	USAttributeComponent* AttributeComp; // 玩家属性组件

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp; // 静态网格体

	// 伤害时闪烁
	UFUNCTION()
	void GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
