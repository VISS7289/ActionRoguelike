// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "DrawDebugHelpers.h"
#include "Component/SAttributeComponent.h"

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

	// ����������ըҩͰ������������˺�
	
	// ��ȡ����������
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttributeComp)
	{
		AttributeComp->ApplyHealthChange(this, -50.0f);
	}


	//LogTemp: OnActorHit in Explosive Barrel
	//LogTemp : Warning: OtherActor: BP_MagicProjectile_C_3, at game time : 24.953748
	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	// ��Ϸ�д�ӡ�ַ���
	FString CombinedString = FString::Printf(TEXT("Hit At Location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}