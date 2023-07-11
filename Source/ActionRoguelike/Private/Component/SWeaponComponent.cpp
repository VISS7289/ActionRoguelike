// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SWeaponComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Item/Projectile/SProjectileBase.h"

// Sets default values for this component's properties
USWeaponComponent::USWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// 枪械网格体
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	WeaponMesh->SetSimulatePhysics(true);
	// 默认7发子弹
	BulletNumMax = 7;


}

// 开火
TSubclassOf<ASProjectileBase> USWeaponComponent::Fire()
{
	// 保证有子弹
	if (BulletInGun.Num() <= 0)
	{
		return nullptr;
	}

	return BulletInGun.Pop();
}

// 装弹
void USWeaponComponent::AddBullet(TSubclassOf<ASProjectileBase> ProjectileClass)
{
	// 不能超上限；
	if (BulletInGun.Num() >= BulletNumMax)
	{
		return;
	}
	// ？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
	if (ProjectileClass)
	{
		BulletInGun.Add(ProjectileClass);
	}
	// ？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
}

// 装满
void USWeaponComponent::AddAll()
{
	// 不能超上限；
	int32 delta = BulletNumMax - BulletInGun.Num();
	if (delta <= 0)
	{
		return;
	}

	if (ensure(DefaultBullet))
	{
		// ？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
		for (int i = 0; i < delta; i++)
		{
			BulletInGun.Add(DefaultBullet);
		}
		// ？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
	}
}


// Called when the game starts
void USWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

