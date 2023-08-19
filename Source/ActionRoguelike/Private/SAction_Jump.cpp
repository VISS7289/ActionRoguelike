// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Jump.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

void USAction_Jump::Initialize_Implementation(USActionComponent* NewActionComp)
{
	Super::Initialize_Implementation(NewActionComp);

	OwningCharacter = Cast<ACharacter>(Owner);
	ensure(OwningCharacter);
}

void USAction_Jump::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	if (ensure(OwningCharacter))
	{
		OwningCharacter->Jump();
	}
	
}

void USAction_Jump::StopAction_Implementation(AActor* InstigatorActor)
{

	if (ensure(OwningCharacter))
	{
		OwningCharacter->StopJumping();
	}
	Super::StopAction_Implementation(InstigatorActor);
}

void USAction_Jump::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction_Jump, OwningCharacter);
}