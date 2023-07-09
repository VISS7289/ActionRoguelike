// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Projectile/SBlackHoleProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"

ASBlackHoleProjectile::ASBlackHoleProjectile()
{
	// ��Ӿ��������
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(RootComponent);
	ForceComp->ForceStrength = -20000000.0f;
	ForceComp->Radius = 1000.0f;

	// �޸���ײ��ֱ��
	SphereComp->SetSphereRadius(120.0f);
}


void ASBlackHoleProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �ݻٿ���ģ�����������
	if (OtherComp->IsSimulatingPhysics("None"))
	{
		OtherActor->Destroy();

	}

	
}