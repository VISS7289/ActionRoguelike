// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SPlayerState.h"
#include "Game/SSaveGame.h"



void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		Credits = SaveObject->Credits;
	}
}


// ��������
void ASPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta))
	{
		return;
	}
	Credits += Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

// ���ּ���
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

