// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Component/SActionComponent.h"
#include "Game/SGameplayFunctionLibrary.h"
#include "Net/UnrealNetwork.h"


void USAction::Initialize_Implementation(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
	Owner = NewActionComp->GetOwner();
}


// 判断是否可以开始
// 根据查看当前标签里是否含有任意BlockedTags，或者Running是否正确
bool USAction::CanStart_Implementation(AActor* InstigatorActor)
{
	USActionComponent* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags) || RepData.Running > 0)
	{
		return false;
	}
	return true;
}

// 开始行动
// 开始行动并且更新标签与RepData
void USAction::StartAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Running:%s"), *GetNameSafe(this));
	//USGameplayFunctionLibrary::LogOnScreen(this, FString::Printf(TEXT("Running:%s"), *GetNameSafe(this)), FColor::Green);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.Running++;
	RepData.InstigatorActor = InstigatorActor;
}

///////////////////////////////////////////////////////////////后期要改///////////////////////////////////////////////////////
// 结束行动
// 结束行动并且更新标签与RepData
void USAction::StopAction_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped:%s"), *GetNameSafe(this));
	//USGameplayFunctionLibrary::LogOnScreen(this, FString::Printf(TEXT("Stopped:%s"), *GetNameSafe(this)), FColor::White);

	//ensure(Running > 0); 后期要改

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	//Running--; 后期要改
	RepData.Running = 0;
	RepData.InstigatorActor = InstigatorActor;
}
///////////////////////////////////////////////////////////////后期要改///////////////////////////////////////////////////////

// 返回UWorld
// 如果正确构造SAction的话，所得Actor必定非空
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

// 返回所在的ActionComponent
USActionComponent* USAction::GetOwningComponent() const
{
	return ActionComp;
}

// 同步数据时
// 数据同步后，根据RepData.Running执行开始或结束
void USAction::OnRep_RepData()
{
	if (RepData.Running)
	{
		StartAction(RepData.InstigatorActor);
	}
	else
	{
		StopAction(RepData.InstigatorActor);
	}
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
}

bool USAction::IsRunning() const
{
	return RepData.Running > 0;
}
