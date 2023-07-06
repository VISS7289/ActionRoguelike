// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SValueChangedWidget.h"
#include "SRageChangedWidget.generated.h"


class USAttributeComponent; // 属性组件

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USRageChangedWidget : public USValueChangedWidget
{
	GENERATED_BODY()

public:

	USRageChangedWidget(const FObjectInitializer& ObjectInitializer);

protected:

	// 绑定事件函数
	virtual void BindWithAttributeChanged(USAttributeComponent* AttributeComp);

	// 值初始化函数
	virtual void ValueInit(USAttributeComponent* AttributeComp);
	
};
