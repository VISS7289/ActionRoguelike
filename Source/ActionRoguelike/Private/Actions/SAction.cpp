// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Component/SActionComponent.h"


void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}


// �ж��Ƿ���Կ�ʼ
// ���ݲ鿴��ǰ��ǩ���Ƿ�������BlockedTags������Running�Ƿ���ȷ
bool USAction::CanStart_Implementation(AActor* InstigatorActor)
{
	USActionComponent* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags) || Running > 0)
	{
		return false;
	}
	return true;
}

// ��ʼ�ж�
// ��ʼ�ж����Ҹ��±�ǩ��Running
void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running:%s"), *GetNameSafe(this));

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	Running++;
}

// �����ж�
// �����ж����Ҹ��±�ǩ��Running
void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped:%s"), *GetNameSafe(this));

	ensure(Running > 0);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	Running--;
}

UWorld* USAction::GetWorld() const
{
	/*UActorComponent* ActorComp = Cast<UActorComponent>(GetOuter());*/
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}


USActionComponent* USAction::GetOwningComponent() const
{
	return ActionComp;
}

bool USAction::IsRunning() const
{
	return Running > 0;
}