// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"



USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_02";
}

// 开始远程攻击行动
// 获取到执行角色后，该角色播放攻击蒙太奇并延时触发攻击
void USAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if (Character)
	{
		if (ensure(AttackAnim))
		{
			Character->PlayAnimMontage(AttackAnim);

			FTimerHandle TimerHandle_AttackDelay;

			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

			float AttackDelay = 0.2f;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackDelay, false);
		}
	}

}

// 实施攻击
// 先用胶囊检测确定屏幕中间可接触到的第一个目标的位置，再从左手发射抛射物
void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileBaseClass))
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

	StopAction(InstigatorCharacter);
}