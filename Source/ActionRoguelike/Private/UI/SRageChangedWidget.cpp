// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SRageChangedWidget.h"
#include "Component/SAttributeComponent.h"


USRageChangedWidget::USRageChangedWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxValue = 220.0f;
	ValueChangeCommon(0.0f);
}

// ���¼�����
void USRageChangedWidget::BindWithAttributeChanged(USAttributeComponent* AttributeComp)
{
	AttributeComp->OnRageChanged.RemoveAll(this);
	AttributeComp->OnRageChanged.AddDynamic(this, &USRageChangedWidget::GetValueChange);
}

// ֵ��ʼ������
void USRageChangedWidget::ValueInit(USAttributeComponent* AttributeComp)
{
	// ���������������UI����
	MaxValue = AttributeComp->GetRageMax();
	ValueChangeCommon(AttributeComp->GetRage());
	ValueMaterial->SetScalarParameterValue("PrograssAlpha", ValueRate);
	
}