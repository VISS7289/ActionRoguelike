// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Component/SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"



// Sets default values
ASProjectileBase::ASProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DestoryPrjDelay = 10.0f; // ������10s
	Damage = 20.0f; // Ĭ���˺�20

	// ����������
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;
	// �����Ч���
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);
	// �������ϵͳ
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);
	// ���Ͷ�����ƶ����
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;

	//SetReplicates(true);

}

// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	// ���Է�����
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	// �����������ټ�ʱ��
	GetWorldTimerManager().SetTimer(FTimerHandle_DestoryPrjDelay, this, &ASProjectileBase::DestoryPrj, DestoryPrjDelay);
}

// Called every frame
void ASProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ע���¼��ص�����
void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// ע�������ײ�¼�������  
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASProjectileBase::OnActorOverlap);
}

// �����ײ�¼�������  
void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ensure(HitEffect))
	{
		Explode();
	}
}

void ASProjectileBase::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ensure(HitEffect) && OtherActor && OtherActor != GetInstigator())
	{
		USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult);
		USGameplayFunctionLibrary::ApplyRage(GetInstigator(), OtherActor, Rage);
		Explode();
	}
}

// ������ײ����Ӧ
void ASProjectileBase::Explode_Implementation()
{
	if (ensure(HitEffect) && ensure(!IsPendingKill()))
	{
		// ������Ч
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation(), GetActorRotation(), FVector(1.0f, 1.0f, 1.0f), true, EPSCPoolMethod::None, true);
		
		// ��ײ����ʧ
		DestoryPrj();
	}
}

// ��������
void ASProjectileBase::DestoryPrj()
{
	// ���������ʱ��
	GetWorldTimerManager().ClearTimer(FTimerHandle_DestoryPrjDelay);
	Destroy();
}