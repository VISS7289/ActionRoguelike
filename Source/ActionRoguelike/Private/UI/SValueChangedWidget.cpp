// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SValueChangedWidget.h"
#include "Component/SAttributeComponent.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetTree.h"
#include "Materials/MaterialInstanceDynamic.h"

// 默认构造方法
USValueChangedWidget::USValueChangedWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxValue = 100.0f;
	ValueChangeCommon(100.0f);
}

// 值初始化函数
void USValueChangedWidget::ValueInit(USAttributeComponent* AttributeComp)
{
	// 根据属性组件更新UI参数
	ValueChangeCommon(AttributeComp->GetHealth());
	MaxValue = Value;
}

// UI构造完成时绑定相关事件
void USValueChangedWidget::NativeConstruct()
{
	// 获取玩家
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (PlayerPawn)
	{
		// 获取玩家属性组件
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(PlayerPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (ensureAlways(AttributeComp))
		{
			ValueInit(AttributeComp);
			// 绑定玩家属性组件 OnHealthChanged 到 GetHealthChange
			//AttributeComp->OnHealthChanged.AddDynamic(this, &USValueChangedWidget::GetValueChange);
			BindWithAttributeChanged(AttributeComp);
		}
	}
}

// 绑定事件函数
void USValueChangedWidget::BindWithAttributeChanged(USAttributeComponent* AttributeComp)
{
	AttributeComp->OnHealthChanged.AddDynamic(this, &USValueChangedWidget::GetValueChange);
}

// 生命值改变时更新UI
void USValueChangedWidget::GetValueChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewValue, float Delta)
{
	ValueChangeCommon(NewValue);

	// 播放PauseHealthAnim动画
	PlayAnimation(PauseValueAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);

	// 血条材质更新（已经设置好了，只需要PrograssAlpha与HealthRate）
	ValueMaterial->SetScalarParameterValue("PrograssAlpha", ValueRate);
}

// 普通更新生命值函数
bool USValueChangedWidget::ValueChangeCommon(float NewValue)
{
	Value = NewValue;
	TValue = FText::FromString(FString::Printf(TEXT("%.2f"), Value));
	ValueRate = Value / MaxValue;

	return true;
}


