// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
	// ����������
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	// ��Ӿ�̬������
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// ȡ����ײ�����ӵ����ڵ�
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;

	SetReplicates(true);
}

// �����ӿ�
void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{

}

// ��ʾҩƿ
void ASPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

// ����ҩƿ״̬
void ASPowerupActor::SetPowerupState(bool bNewIsActive)
{
	// ������ײ
	SetActorEnableCollision(bNewIsActive);
	// ���ÿɼ���
	RootComponent->SetVisibility(bNewIsActive, true);
}

// ����ҩƿ��������ȴ
void ASPowerupActor::HideAndCooldownPowerup()
{
	// ����״̬���������ɵ���ʱ��
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPowerupActor::ShowPowerup, RespawnTime);
}

