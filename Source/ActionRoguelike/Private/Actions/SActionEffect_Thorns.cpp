// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SActionEffect_Thorns.h"
#include "Component/SAttributeComponent.h"
#include "Component/SActionComponent.h"
#include "Game/SGameplayFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	ReflectFraction = 0.2f;

	// ����Ҫ������ʱ��
	Duration = 0.0f;
	Period = 0.0f;
}

// Action��ʼʱ����������ֵ�ı��¼�
void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

// Action����ʱ��ȡ����������ֵ�ı��¼�
void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

// ����ֵ�ı�ʱ����
// �Ե��˵��˺������������
void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// ���˺�����Դ�����Լ�
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// ������ˣ�����ȡ��
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}
		ReflectedAmount = FMath::Abs(ReflectedAmount);
		// �����˺�
		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}