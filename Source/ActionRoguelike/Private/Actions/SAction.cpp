// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Component/SActionComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Net/UnrealNetwork.h"


void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}


// 判断是否可以开始
// 根据查看当前标签里是否含有任意BlockedTags，或者Running是否正确
bool USAction::CanStart_Implementation(AActor* InstigatorActor)
{
	USActionComponent* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags) || Running > 0)
	{
		return false;
	}
	return true;
}

// 开始行动
// 开始行动并且更新标签与Running
void USAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running:%s"), *GetNameSafe(this));
	USGameplayFunctionLibrary::LogOnScreen(this, FString::Printf(TEXT("Running:%s"), *GetNameSafe(this)), FColor::Green);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	Running++;
}

///////////////////////////////////////////////////////////////后期要改///////////////////////////////////////////////////////
// 结束行动
// 结束行动并且更新标签与Running
void USAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Stopped:%s"), *GetNameSafe(this));
	USGameplayFunctionLibrary::LogOnScreen(this, FString::Printf(TEXT("Stopped:%s"), *GetNameSafe(this)), FColor::White);

	//ensure(Running > 0); 后期要改

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	//Running--; 后期要改
	Running = 0;
}
///////////////////////////////////////////////////////////////后期要改///////////////////////////////////////////////////////

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

void USAction::OnRep_IsRunning()
{
	if (Running)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, Running);
	DOREPLIFETIME(USAction, ActionComp);
}

bool USAction::IsRunning() const
{
	return Running > 0;
}