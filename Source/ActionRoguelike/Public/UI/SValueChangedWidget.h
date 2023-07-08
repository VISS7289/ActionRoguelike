// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SValueChangedWidget.generated.h"

class USAttributeComponent; // 属性组件
class UWidgetAnimation; // UI动画
class UMaterialInstanceDynamic; // 动态材质接口

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USValueChangedWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	USValueChangedWidget(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	FText TValue; // 值Text格式

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	float Value; // 值

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	float MaxValue; // 最大值

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	float ValueRate; // 值百分比

	UPROPERTY(BlueprintReadWrite, Category = "Player Health")
	UMaterialInstanceDynamic* ValueMaterial; // 材质

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UWidgetAnimation* PauseValueAnim; // 值变化动画

protected:

	// UI构造完成时绑定相关事件
	virtual void NativeConstruct() override;

	// 生命值改变时更新UI
	UFUNCTION(BlueprintCallable)
	void GetValueChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewValue, float Delta);

	// 玩家改变时更新UI
	UFUNCTION(BlueprintCallable)
	void GetPlayerChange(APawn* InPawn);

	// 普通更新生命值函数
	bool ValueChangeCommon(float NewValue);

	// 值初始化函数
	virtual void ValueInit(USAttributeComponent* AttributeComp);

	// 绑定事件函数
	virtual void BindWithAttributeChanged(USAttributeComponent* AttributeComp);
	
};
