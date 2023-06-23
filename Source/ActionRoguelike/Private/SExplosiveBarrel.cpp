// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	// ���ըҩͰ������
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;
	// ��Ӿ��������
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);
	// �����Զ�����Ϊ false  
	ForceComp->SetAutoActivate(false); 
	// ���þ���������İ뾶������ǿ�� 
	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;
	// ���þ���������Ƿ�ı������ٶ�  
	ForceComp->bImpulseVelChange = true; 
	// ������ײͨ��  
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// ע���¼��ص�����
void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// ע�������ײ�¼�������  
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

// �����ײ�¼�������  
void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// �������������������  
	ForceComp->FireImpulse();
}