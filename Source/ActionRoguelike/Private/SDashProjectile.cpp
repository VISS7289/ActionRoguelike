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
	// ���ñ�ը��ʱ��
	GetWorldTimerManager().SetTimer(FTimerHandle_DetonateDelay, this, &ASDashProjectile::Explode, DetonateDelay);
}

// ���ر�ը
void ASDashProjectile::Explode_Implementation()
{
	// �����ը��ʱ��
	GetWorldTimerManager().ClearTimer(FTimerHandle_DetonateDelay);
	// ��������Ч��
	UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, GetActorLocation(), GetActorRotation());
	// ��������ϵͳ
	EffectComp->DeactivateSystem();
	// ֹͣģ������ȡ����ײ
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	// ���ô�����ʱ��
	FTimerHandle FTimerHandle_TeleportDelay;
	GetWorldTimerManager().SetTimer(FTimerHandle_TeleportDelay, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);

}
// ʵ�ִ���
void ASDashProjectile::TeleportInstigator()
{
	// ��ȡ������
	AActor* ActorToTeleport = GetInstigator();
	if (ensureAlways(ActorToTeleport))
	{
		// ���ͷ�����
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
}