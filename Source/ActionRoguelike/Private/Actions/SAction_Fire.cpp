// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_Fire.h"
#include "GameFramework/Character.h"
#include "Component/SWeaponComponent.h"
#include "Component/SActionComponent.h"


// ��ʼԶ�̹����ж�
// ��ȡ��ִ�н�ɫ�󣬽������ִ�иý�ɫ���Ź�����̫�沢��ʱ��������
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