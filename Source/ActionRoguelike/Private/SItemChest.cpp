// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 添加箱子网格体组件
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;
	// 添加盖子网格体组件
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);
	// 添加宝藏网格体组件
	Treasure = CreateDefaultSubobject<UStaticMeshComponent>("Treasure");
	Treasure->SetupAttachment(BaseMesh);
	// 添加粒子系统
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(Treasure);

	SetReplicates(true);
}

// 交互接口
void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void ASItemChest::OnRep_LidOpened()
{
	float CurPitch = bLidOpened ? TargetPitch : 0;
	FVector CurTransform = bLidOpened ? TargetTransform : FVector(0, 0, 0);

	LidMesh->SetRelativeLocation(CurTransform); // 盖子移动
	LidMesh->SetRelativeRotation(FRotator(CurPitch, 0, 0)); // 盖子旋转
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpened);
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

