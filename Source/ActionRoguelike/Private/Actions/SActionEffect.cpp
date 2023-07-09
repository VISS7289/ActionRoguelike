// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SActionEffect.h"
#include "TimerManager.h"
#include "Component/SActionComponent.h"



USActionEffect::USActionEffect()
{
	bAutoStart = true; // BUFF默认自动启动
}

// BUFF启动
// 设置持续时间与周期效果的延时器
void USActionEffect::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);
	// 持续时间,到达一定时间后停止
	if (Duration > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", InstigatorActor);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}
	// 周期效果，到达一定时间后执行周期效果
	if (Period > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", InstigatorActor);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

// BUFF结束
// 清除延时器与该行动
void USActionEffect::StopAction_Implementation(AActor* InstigatorActor)
{
	// 比如周期3s，总时长也是3s，这样就会在3s的时候执行效果再结束
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(InstigatorActor);
	}

	Super::StopAction_Implementation(InstigatorActor);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	USActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}

// BUFF周期效果执行
// 在子类中重载
void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* InstigatorActor)
{

}