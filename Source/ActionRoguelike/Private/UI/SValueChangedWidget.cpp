// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SValueChangedWidget.h"
#include "Component/SAttributeComponent.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetTree.h"
#include "Materials/MaterialInstanceDynamic.h"

// Ĭ�Ϲ��췽��
USValueChangedWidget::USValueChangedWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxValue = 100.0f;
	ValueChangeCommon(100.0f);
}

// ֵ��ʼ������
void USValueChangedWidget::ValueInit(USAttributeComponent* AttributeComp)
{
	// ���������������UI����
	ValueChangeCommon(AttributeComp->GetHealth());
	MaxValue = Value;
}

// UI�������ʱ������¼�
void USValueChangedWidget::NativeConstruct()
{
	// ��ȡ���
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (PlayerPawn)
	{
		// ��ȡ����������
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(PlayerPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (ensureAlways(AttributeComp))
		{
			ValueInit(AttributeComp);
			// ������������ OnHealthChanged �� GetHealthChange
			//AttributeComp->OnHealthChanged.AddDynamic(this, &USValueChangedWidget::GetValueChange);
			BindWithAttributeChanged(AttributeComp);
		}
	}
}

// ���¼�����
void USValueChangedWidget::BindWithAttributeChanged(USAttributeComponent* AttributeComp)
{
	AttributeComp->OnHealthChanged.AddDynamic(this, &USValueChangedWidget::GetValueChange);
}

// ����ֵ�ı�ʱ����UI
void USValueChangedWidget::GetValueChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewValue, float Delta)
{
	ValueChangeCommon(NewValue);

	// ����PauseHealthAnim����
	PlayAnimation(PauseValueAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);

	// Ѫ�����ʸ��£��Ѿ����ú��ˣ�ֻ��ҪPrograssAlpha��HealthRate��
	ValueMaterial->SetScalarParameterValue("PrograssAlpha", ValueRate);
}

// ��ͨ��������ֵ����
bool USValueChangedWidget::ValueChangeCommon(float NewValue)
{
	Value = NewValue;
	TValue = FText::FromString(FString::Printf(TEXT("%.2f"), Value));
	ValueRate = Value / MaxValue;

	return true;
}


