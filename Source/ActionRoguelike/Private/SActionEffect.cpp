// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"
#include "TimerManager.h"
#include "Component/SActionComponent.h"



USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (Duration > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", InstigatorActor);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", InstigatorActor);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* InstigatorActor)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(InstigatorActor);
	}

	Super::StopAction_Implementation(InstigatorActor);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	USActionComponent* ActionComp = GetOwningComponent();
	if (ActionComp)
	{
		ActionComp->RemoveAction(this);
	}
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* InstigatorActor)
{

}