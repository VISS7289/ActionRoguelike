// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_Fire.h"
#include "GameFramework/Character.h"
#include "Component/SWeaponComponent.h"
#include "Component/SActionComponent.h"


// 开始远程攻击行动
// 获取到执行角色后，仅服务端执行该角色播放攻击蒙太奇并延时触发攻击
void USAction_Fire::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	FireEnd = false;

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if (Character)
	{
		if (ensure(AttackAnim))
		{
			PlayFireAnim();
		}
	}

}

void USAction_Fire::FireAnimEnd(UAnimMontage* Montage, bool bInterrupted)
{
	Super::FireAnimEnd(Montage, bInterrupted);

	if (isLoop && !FireEnd && ensure(WeaponComp) && WeaponComp->CanFire())
	{
		StartAction(FireCharacter);
	}
	else {
		StopAction(FireCharacter);
	}
	
}

void USAction_Fire::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);

	FireEnd = true;
}