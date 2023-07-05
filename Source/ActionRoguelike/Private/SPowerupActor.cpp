// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
	// 添加球体组件
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	// 添加静态网格体
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// 取消碰撞并连接到根节点
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;

	SetReplicates(true);
}

// 交互接口
void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{

}

// 显示药瓶
void ASPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

// 设置药瓶状态
void ASPowerupActor::SetPowerupState(bool bNewIsActive)
{
	// 设置碰撞
	SetActorEnableCollision(bNewIsActive);
	// 设置可见性
	RootComponent->SetVisibility(bNewIsActive, true);
}

// 隐藏药瓶并设置冷却
void ASPowerupActor::HideAndCooldownPowerup()
{
	// 设置状态与重新生成的延时器
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPowerupActor::ShowPowerup, RespawnTime);
}

