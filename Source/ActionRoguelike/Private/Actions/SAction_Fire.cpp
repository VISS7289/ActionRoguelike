// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_Fire.h"
#include "GameFramework/Character.h"


// 开始远程攻击行动
// 获取到执行角色后，仅服务端执行该角色播放攻击蒙太奇并延时触发攻击
void USAction_Fire::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if (Character)
	{
		if (ensure(AttackAnim))
		{
			Character->PlayAnimMontage(AttackAnim);

			if (Character->HasAuthority())
			{
				FTimerHandle TimerHandle_AttackDelay;
				FTimerDelegate Delegate;
				Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

				float AttackDelay = 0.2f;

				GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackDelay, false);
			}

		}
	}

}

// 攻击结束后结束Action
void USAction_Fire::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	Super::AttackDelay_Elapsed(InstigatorCharacter);

	StopAction(InstigatorCharacter);
}
