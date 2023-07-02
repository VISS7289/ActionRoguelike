// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "DrawDebugHelpers.h"
#include "Component/SAttributeComponent.h"

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

	// 如果玩家碰到炸药桶，则对玩家造成伤害
	
	// 获取玩家属性组件
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttributeComp)
	{
		AttributeComp->ApplyHealthChange(this, -50.0f);
	}


	//LogTemp: OnActorHit in Explosive Barrel
	//LogTemp : Warning: OtherActor: BP_MagicProjectile_C_3, at game time : 24.953748
	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	// 游戏中打印字符串
	FString CombinedString = FString::Printf(TEXT("Hit At Location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}