// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "SAttributeComponent.h"


// Sets default values
ASProjectileBase::ASProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DestoryPrjDelay = 10.0f; // ������10s

	// ����������
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	// �������ϵͳ
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);
	// ���Ͷ�����ƶ����
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;

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
}

// �����ײ�¼�������  
void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ensureAlways(HitEffect) && OtherActor != GetInstigator())
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), 10.0f, 32, FColor::Green, false, 2.0f); // Debug��
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);
		}
		Explode();
	}
}

// ������ײ����Ӧ
void ASProjectileBase::Explode_Implementation()
{
	if (ensureAlways(HitEffect) && ensureAlways(!IsPendingKill()))
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