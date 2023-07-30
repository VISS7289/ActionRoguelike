// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_Cold.h"

#include "Engine/World.h"

USAction_Cold::USAction_Cold()
{
	ClodTime = 0.0f;
	InCold = false;
}

void USAction_Cold::InitCold(float NewCloadTime)
{
	ClodTime = NewCloadTime;
	ColdWorld = GetWorld();
	ensure(ColdWorld);
}

void USAction_Cold::StartCold()
{
	if (ClodTime > 0)
	{
		InCold = true;
		ColdWorld->GetTimerManager().SetTimer(FTimerHandle_ColdDelay, this, &USAction_Cold::ColdFinish, ClodTime);
	}
}

bool USAction_Cold::IsInCold()
{
	return InCold;
}

void USAction_Cold::ColdFinish()
{
	InCold = false;
}

void USAction_Cold::SetCloadTime(float NewCloadTime)
{
	ClodTime = NewCloadTime;
}

float USAction_Cold::GetCloadTime()
{
	return ClodTime;
}