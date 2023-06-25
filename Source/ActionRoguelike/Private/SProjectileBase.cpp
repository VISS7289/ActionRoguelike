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
	DestoryPrjDelay = 10.0f; // 最多存在10s

	// 添加球体组件
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	// 添加粒子系统
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);
	// 添加投射物移动组件
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
	// 忽略发射者
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	// 设置自我销毁计时器
	GetWorldTimerManager().SetTimer(FTimerHandle_DestoryPrjDelay, this, &ASProjectileBase::DestoryPrj, DestoryPrjDelay);
}

// Called every frame
void ASProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 注册事件回调函数
void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 注册组件碰撞事件处理函数  
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
}

// 组件碰撞事件处理函数  
void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ensureAlways(HitEffect) && OtherActor != GetInstigator())
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), 10.0f, 32, FColor::Green, false, 2.0f); // Debug球
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);
		}
		Explode();
	}
}

// 常规碰撞后响应
void ASProjectileBase::Explode_Implementation()
{
	if (ensureAlways(HitEffect) && ensureAlways(!IsPendingKill()))
	{
		// 生成特效
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation(), GetActorRotation(), FVector(1.0f, 1.0f, 1.0f), true, EPSCPoolMethod::None, true);
		
		// 碰撞后消失
		DestoryPrj();
	}
}

// 自我销毁
void ASProjectileBase::DestoryPrj()
{
	// 清除销毁延时器
	GetWorldTimerManager().ClearTimer(FTimerHandle_DestoryPrjDelay);
	Destroy();
}