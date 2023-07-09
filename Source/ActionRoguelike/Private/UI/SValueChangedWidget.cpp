// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SValueChangedWidget.h"
#include "Component/SAttributeComponent.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetTree.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Player/SPlayerController.h"
#include "GameFramework/PlayerController.h"



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
	MaxValue = Value;
	ValueChangeCommon(AttributeComp->GetHealth());
	// Ѫ�����ʸ��£��Ѿ����ú��ˣ�ֻ��ҪPrograssAlpha��HealthRate��
	ValueMaterial->SetScalarParameterValue("PrograssAlpha", ValueRate);
}

// UI�������ʱ������¼�
void USValueChangedWidget::NativeConstruct()
{
	// ��ȡ���
	ASPlayerController* Controller = Cast<ASPlayerController>(GetOwningPlayer());
	if (Controller)
	{
		Controller->OnPawnChanged.AddDynamic(this, &USValueChangedWidget::GetPlayerChange);
		GetPlayerChange(Controller->GetPawn());
	}
	
}

// ���¼�����
void USValueChangedWidget::BindWithAttributeChanged(USAttributeComponent* AttributeComp)
{
	AttributeComp->OnHealthChanged.RemoveAll(this);
	AttributeComp->OnHealthChanged.AddDynamic(this, &USValueChangedWidget::GetValueChange);
}

// ����ֵ�ı�ʱ����UI
void USValueChangedWidget::GetValueChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewValue, float Delta)
{
	ValueChangeCommon(NewValue);
	
	if (Delta != 0)
	{
		// ����PauseHealthAnim����
		PlayAnimation(PauseValueAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	}
	

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

// ��Ҹı�ʱ����UI
void USValueChangedWidget::GetPlayerChange(APawn* InPawn)
{
	if (InPawn)
	{
		// ��ȡ����������
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (ensureAlways(AttributeComp))
		{
			ValueInit(AttributeComp);
			// ������������ OnHealthChanged �� GetHealthChange
			//AttributeComp->OnHealthChanged.AddDynamic(this, &USValueChangedWidget::GetValueChange);
			BindWithAttributeChanged(AttributeComp);
			
		}
	}
}
