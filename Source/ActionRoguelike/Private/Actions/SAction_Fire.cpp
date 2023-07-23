// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_Fire.h"
#include "GameFramework/Character.h"


// ��ʼԶ�̹����ж�
// ��ȡ��ִ�н�ɫ�󣬽������ִ�иý�ɫ���Ź�����̫�沢��ʱ��������
void USAction_Fire::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if (Character)
	{
		if (ensure(AttackAnim))
		{
			PlayFireAnim();
			StopAction(InstigatorActor);
		}
	}

}