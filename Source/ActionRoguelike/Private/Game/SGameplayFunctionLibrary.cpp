// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameplayFunctionLibrary.h"
#include "Component/SAttributeComponent.h"
#include "Components/PrimitiveComponent.h"

// ����˺�����
// ����TargetActor��Ŀ�꣩����������������ı亯������˺�
bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(TargetActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

// �з�����˺�����
// ����˺���ͬʱ��Ŀ��ʩ�ӳ��
bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}

// ���ŭ������
// ����TargetActor��Ŀ�꣩�����������ŭ���ı亯��
bool USGameplayFunctionLibrary::ApplyRage(AActor* RageCauser, AActor* TargetActor, float RageAmount)
{
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(TargetActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttributeComp)
	{
		return AttributeComp->ApplyRage(RageCauser, RageAmount);
	}
	return false;
}

// ����״̬�µ���־��ӡ
void USGameplayFunctionLibrary::LogOnScreen(UObject* WorldContext, FString Msg, FColor Color, float Duration)
{
	if (!ensure(WorldContext))
	{
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}

	FString NetPrefix = World->IsNetMode(NM_Client) ? "[Client]" : "[Server]";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
	}
}