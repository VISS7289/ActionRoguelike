// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Component/SAttributeComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	// 忽略发射者
	SphereComp->IgnoreActorWhenMoving(GetInstigator(),true);
	
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// 注册事件回调函数
void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 注册组件碰撞事件处理函数  
	SphereComp->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnActorHit);
}

// 组件碰撞事件处理函数  
void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ensureAlways(HitEffect) && OtherActor != GetInstigator())
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), 10.0f, 32, FColor::Green, false, 2.0f); // Debug球
		// 生成特效
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation(), GetActorRotation(), FVector(1.0f, 1.0f, 1.0f), true, EPSCPoolMethod::None, true);
		// 碰撞后消失
		// 获取玩家属性组件
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -20.0f);
		}

		Destroy();
	}
}