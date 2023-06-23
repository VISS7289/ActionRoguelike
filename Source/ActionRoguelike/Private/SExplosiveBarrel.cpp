// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	// 添加炸药桶网格体
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;
	// 添加径向力组件
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);
	// 设置自动激活为 false  
	ForceComp->SetAutoActivate(false); 
	// 设置径向力组件的半径和脉冲强度 
	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;
	// 设置径向力组件是否改变脉冲速度  
	ForceComp->bImpulseVelChange = true; 
	// 设置碰撞通道  
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// 注册事件回调函数
void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// 注册组件碰撞事件处理函数  
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

// 组件碰撞事件处理函数  
void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 触发辐射力组件的脉冲  
	ForceComp->FireImpulse();
}