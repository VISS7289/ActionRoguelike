// Fill out your copyright notice in the Description page of Project Settings.
// 该类为敌人血条类
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"


class USizeBox;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset; // 血条偏移

	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox; // 盒子组件

	// 每帧修正敌人血条位置
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (ExposeOnSpawn = true))
	AActor* AttacheActor; // 附着者
	
};
