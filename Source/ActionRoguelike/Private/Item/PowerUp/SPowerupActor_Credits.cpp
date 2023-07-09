// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PowerUp/SPowerupActor_Credits.h"
#include "Player/SPlayerState.h"

ASPowerupActor_Credits::ASPowerupActor_Credits()
{
	CreditsAmount = 80;
}

// ¼Ó»ý·Ö
void ASPowerupActor_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		PS->AddCredits(CreditsAmount);
		HideAndCooldownPowerup();
	}
}