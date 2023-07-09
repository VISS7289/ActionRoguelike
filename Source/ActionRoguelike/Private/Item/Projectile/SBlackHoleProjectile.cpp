// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Projectile/SBlackHoleProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"

ASBlackHoleProjectile::ASBlackHoleProjectile()
{
	// 添加径向力组件
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(RootComponent);
	ForceComp->ForceStrength = -20000000.0f;
	ForceComp->Radius = 1000.0f;

	// 修改碰撞球直径
	SphereComp->SetSphereRadius(120.0f);
}


void ASBlackHoleProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 摧毁可以模拟物理的物体
	if (OtherComp->IsSimulatingPhysics("None"))
	{
		OtherActor->Destroy();

	}

	
}