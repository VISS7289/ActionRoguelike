// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Jump.h"
#include "GameFramework/Character.h"

void USAction_Jump::Initialize_Implementation(USActionComponent* NewActionComp)
{
	Super::Initialize_Implementation(NewActionComp);

	OwningCharacter = Cast<ACharacter>(Owner);
	ensure(OwningCharacter);
}

void USAction_Jump::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	OwningCharacter->Jump();
}

void USAction_Jump::StopAction_Implementation(AActor* InstigatorActor)
{
	OwningCharacter->StopJumping();

	Super::StopAction_Implementation(InstigatorActor);
}