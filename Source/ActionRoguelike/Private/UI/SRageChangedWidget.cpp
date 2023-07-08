// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRageChangedWidget.h"
#include "Component/SAttributeComponent.h"


USRageChangedWidget::USRageChangedWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxValue = 220.0f;
	ValueChangeCommon(0.0f);
}

// 绑定事件函数
void USRageChangedWidget::BindWithAttributeChanged(USAttributeComponent* AttributeComp)
{
	AttributeComp->OnRageChanged.RemoveAll(this);
	AttributeComp->OnRageChanged.AddDynamic(this, &USRageChangedWidget::GetValueChange);
}

// 值初始化函数
void USRageChangedWidget::ValueInit(USAttributeComponent* AttributeComp)
{
	// 根据属性组件更新UI参数
	MaxValue = AttributeComp->GetRageMax();
	ValueChangeCommon(AttributeComp->GetRage());
	ValueMaterial->SetScalarParameterValue("PrograssAlpha", ValueRate);
	
}