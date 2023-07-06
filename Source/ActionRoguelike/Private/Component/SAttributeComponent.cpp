// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SAttributeComponent.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

// �˺��������״���
static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier For Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
	HealthMax = 100;
	Rage = 0;
	RageMax = 220;

	SetIsReplicatedByDefault(true);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);

	//DOREPLIFETIME_CONDITION(USActionComponent, HealthMax, COND_InitialOnly);
}


// ɱ������
bool USAttributeComponent::Kill(AActor* InstigatordActor)
{
	return ApplyHealthChange(InstigatordActor, -GetHealthMax());
}

// ����ֵ�ı�
// ���ݱ仯��Delta��������ϢCVarDamageMultiplier��������ֵ�������������д����ڴ���ʱ��㲥OnHealthChanged��
bool USAttributeComponent::ApplyHealthChange(AActor* InstigatordActor, float Delta)
{

	if (Delta < 0.0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}

	float OldHealth = Health; // ��ȥ����ֵ
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float ActualDelta = Health - OldHealth; // ʵ�ʱ仯��
	//OnHealthChanged.Broadcast(InstigatordActor, this, Health, ActualDelta);
	if (ActualDelta != 0)
	{
		MulticastHealthChanged(InstigatordActor, Health, ActualDelta);
	}

	if (Delta < 0.0f && Health <= 0.0f)
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatordActor);
		}
		
	}

	return ActualDelta != 0;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatordActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatordActor, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatordActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatordActor, this, NewRage, Delta);
}

// ��������ֵ
float USAttributeComponent::GetHealth()
{
	return Health;
}

// �������ֵ��ȡ
float USAttributeComponent::GetHealthMax()
{
	return HealthMax;
}

// �Ƿ����
bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

// �Ƿ���Ѫ
bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

// ����Actor�ϵ�USAttributeComponent
USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

// �ж�Actor�Ƿ����
bool USAttributeComponent::IsActorAlive(AActor* FromActor)
{
	USAttributeComponent* AttributeComp = GetAttributes(FromActor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

// ŭ����ȡ
float USAttributeComponent::GetRage() const
{
	return Rage;
}

// �ı�ŭ��
// ���ݱ仯��Delta����ŭ�����㲥OnRageChanged��
bool USAttributeComponent::ApplyRage(AActor* InstigatordActor, float Delta)
{

	float OldRage = Rage; // ��ȥ����ֵ
	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);
	float ActualDelta = Rage - OldRage; // ʵ�ʱ仯��
	//OnHealthChanged.Broadcast(InstigatordActor, this, Health, ActualDelta);
	if (ActualDelta != 0)
	{
		MulticastRageChanged(InstigatordActor, Rage, ActualDelta);
	}

	return ActualDelta != 0;
}

float USAttributeComponent::GetRageMax() const
{
	return RageMax;
}