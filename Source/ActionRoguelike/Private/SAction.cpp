// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running:%s"), *GetNameSafe(this));
}


void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped:%s"), *GetNameSafe(this));
}

UWorld* USAction::GetWorld() const
{
	UActorComponent* ActorComp = Cast<UActorComponent>(GetOuter());
	if (ActorComp)
	{
		return ActorComp->GetWorld();
	}
	return nullptr;
}