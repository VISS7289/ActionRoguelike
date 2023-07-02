// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Component/SActionComponent.h"


// 判断是否可以开始
// 根据查看当前标签里是否含有任意BlockedTags，或者Running是否正确
bool USAction::CanStart_Implementation(AActor* InstigatorActor)
{
	USActionComponent* ActionComp = GetOwningComponent();

	if (ActionComp->ActiveGameplayTags.HasAny(BlockedTags) || Running < 0)
	{
		return false;
	}
	return true;
}

// 开始行动
// 开始行动并且更新标签与Running
void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running:%s"), *GetNameSafe(this));

	USActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);

	Running++;
}

// 结束行动
// 结束行动并且更新标签与Running
void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped:%s"), *GetNameSafe(this));

	ensure(Running > 0);

	USActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);

	Running--;
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


USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

bool USAction::IsRunning() const
{
	return Running > 0;
}