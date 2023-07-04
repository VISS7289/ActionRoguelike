// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"



// 积分增加
void ASPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta))
	{
		return;
	}
	Credits += Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

// 积分减少
bool ASPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta))
	{
		return false;
	}
	if (Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
	return true;
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}