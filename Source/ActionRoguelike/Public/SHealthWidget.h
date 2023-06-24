// Fill out your copyright notice in the Description page of Project Settings.
// 这个类是玩家血条
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHealthWidget.generated.h"

class USAttributeComponent; // 属性组件
class UWidgetAnimation; // UI动画
class UMaterialInstanceDynamic; // 动态材质接口

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	USHealthWidget(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	FText THealth; // 生命值

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	float Health; // 生命值

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	float MaxHealth; // 最大生命值

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	float HealthRate; // 生命值百分比

	UPROPERTY(BlueprintReadWrite, Category = "Player Health")
	UMaterialInstanceDynamic* HealthMaterial; // 血条材质

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UWidgetAnimation* PauseHealthAnim; // 生命值减少动画
	
protected:

	// UI构造完成时绑定相关事件
	virtual void NativeConstruct() override;

	// 生命值改变时更新UI
	UFUNCTION(BlueprintCallable)
	void GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	// 普通更新生命值函数
	bool HealthChangeCommon(float NewHealth);
	
};
