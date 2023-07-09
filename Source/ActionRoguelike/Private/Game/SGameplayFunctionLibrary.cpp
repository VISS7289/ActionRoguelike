// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameplayFunctionLibrary.h"
#include "Component/SAttributeComponent.h"
#include "Components/PrimitiveComponent.h"

// 造成伤害函数
// 调用TargetActor（目标）的属性组件的生命改变函数造成伤害
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

// 有方向的伤害函数
// 造成伤害的同时给目标施加冲击
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

// 造成怒气函数
// 调用TargetActor（目标）的属性组件的怒气改变函数
bool USGameplayFunctionLibrary::ApplyRage(AActor* RageCauser, AActor* TargetActor, float RageAmount)
{
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(TargetActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttributeComp)
	{
		return AttributeComp->ApplyRage(RageCauser, RageAmount);
	}
	return false;
}

// 多人状态下的日志打印
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