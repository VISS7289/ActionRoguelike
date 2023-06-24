// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MovementComp->InitialSpeed = 6000.0f;
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	// 设置爆炸延时器
	GetWorldTimerManager().SetTimer(FTimerHandle_DetonateDelay, this, &ASDashProjectile::Explode, DetonateDelay);
}

// 重载爆炸
void ASDashProjectile::Explode_Implementation()
{
	// 清除爆炸延时器
	GetWorldTimerManager().ClearTimer(FTimerHandle_DetonateDelay);
	// 生成粒子效果
	UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, GetActorLocation(), GetActorRotation());
	// 禁用粒子系统
	EffectComp->DeactivateSystem();
	// 停止模拟物理取消碰撞
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	// 设置传送延时器
	FTimerHandle FTimerHandle_TeleportDelay;
	GetWorldTimerManager().SetTimer(FTimerHandle_TeleportDelay, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);

}
// 实现传送
void ASDashProjectile::TeleportInstigator()
{
	// 获取发送者
	AActor* ActorToTeleport = GetInstigator();
	if (ensureAlways(ActorToTeleport))
	{
		// 传送发送者
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
}