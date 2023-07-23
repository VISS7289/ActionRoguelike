// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SActionWeapon.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Component/SWeaponComponent.h"
#include "Component/SActionComponent.h"
#include "Item/Projectile/SProjectileBase.h"



USActionWeapon::USActionWeapon()
{
	HandSocketName = "Muzzle_02"; // 左手发射骨骼名称
}

void USActionWeapon::Initialize_Implementation(USActionComponent* NewActionComp)
{
	Super::Initialize_Implementation(NewActionComp);

	FireCharacter = Cast<ACharacter>(Owner);
	CharacterAnimIns = FireCharacter->GetMesh()->GetAnimInstance();
	WeaponComp = USWeaponComponent::GetWeaponComp(FireCharacter);
	ensure(CharacterAnimIns);
	ensure(WeaponComp);
}

// 判断是否可以攻击
bool USActionWeapon::CanStart_Implementation(AActor* InstigatorActor)
{
	if (!Super::CanStart_Implementation(InstigatorActor))
	{
		GetOwningComponent()->StopActionByName(InstigatorActor, "Accumulate");
		return false;
	}

	if (ensure(WeaponComp))
	{
		if (!WeaponComp->CanFire())
		{
			GetOwningComponent()->StartActionByName(InstigatorActor, "Reload");
			return false;
		}
		return true;
	}

	return false;
}

// 实施攻击
// 先用胶囊检测确定屏幕中间可接触到的第一个目标的位置，再从左手发射抛射物
void USActionWeapon::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{

	TSubclassOf<ASProjectileBase> ProjectileBaseClass = WeaponComp->Fire(); // 发射物

	if (ProjectileBaseClass)
	{
		// 左手位置
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		// 胶囊检测体设置
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		// 设置碰撞
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);
		// 设置检测物
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		// 设置起点终点
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);
		// 胶囊检测
		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		// 根据检测结果计算旋转方向
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		// 角色前向位置
		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		// 设置生成物规则
		FActorSpawnParameters SpawnParames;
		SpawnParames.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParames.Instigator = InstigatorCharacter;
		// 生成抛射物
		GetWorld()->SpawnActor<AActor>(ProjectileBaseClass, SpawnTM, SpawnParames);
	}
}

void USActionWeapon::PlayFireAnim()
{
	if (ensure(CharacterAnimIns) && ensure(AttackAnim))
	{
		CharacterAnimIns->Montage_Play(AttackAnim);
		CharacterAnimIns->OnPlayMontageNotifyBegin.AddDynamic(this, &USActionWeapon::FireNotify);
	}
}

void USActionWeapon::FireNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	CharacterAnimIns->OnPlayMontageNotifyBegin.RemoveDynamic(this, &USActionWeapon::FireNotify);

	// 发射子弹
	if (ensure(FireCharacter))
	{
		AttackDelay_Elapsed(FireCharacter);
	}
}