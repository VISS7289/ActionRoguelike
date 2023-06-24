// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthWidget.h"
#include "SAttributeComponent.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetTree.h"
#include "Materials/MaterialInstanceDynamic.h"

// Ĭ�Ϲ��췽��
USHealthWidget::USHealthWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HealthChangeCommon(100.0f);
	MaxHealth = Health;
}

// UI�������ʱ������¼�
void USHealthWidget::NativeConstruct()
{

	// ��ȡ���
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (PlayerPawn)
	{
		// ��ȡ����������
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(PlayerPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (ensureAlways(AttributeComp))
		{
			// ���������������UI����
			HealthChangeCommon(AttributeComp->GetHealth());
			MaxHealth = Health;
			// ������������ OnHealthChanged �� GetHealthChange
			AttributeComp->OnHealthChanged.AddDynamic(this, &USHealthWidget::GetHealthChange);
		}
	}
}

// ����ֵ�ı�ʱ����UI
void USHealthWidget::GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	HealthChangeCommon(NewHealth);

	// ����PauseHealthAnim����
	PlayAnimation(PauseHealthAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);

	// Ѫ�����ʸ��£��Ѿ����ú��ˣ�ֻ��ҪPrograssAlpha��HealthRate��
	HealthMaterial->SetScalarParameterValue("PrograssAlpha", HealthRate);
}

// ��ͨ��������ֵ����
bool USHealthWidget::HealthChangeCommon(float NewHealth)
{
	Health = NewHealth;
	THealth = FText::FromString(FString::Printf(TEXT("%.2f"), Health));
	HealthRate = Health / MaxHealth;

	return true;
}
