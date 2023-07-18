// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SActionComponent.h"
#include "Actions/SAction.h"
#include "Game/SGameplayFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"


// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}

// 开始游戏时仅在服务器初始化Actions
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// 只在服务器初始化
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
	
}


// Debug查看Action状态
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	//// Debug查看Action状态
	//for (USAction* Action : Actions)
	//{
	//	FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
	//	FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s ; Outer: %s"),
	//		*GetNameSafe(GetOwner()),
	//		*GetNameSafe(Action),
	//		*GetNameSafe(Action->GetOuter()));

	//	USGameplayFunctionLibrary::LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	//}
}

// 添加行动
// 服务器根据类引用来添加具体的行动
void USActionComponent::AddAction(AActor* InstigatorActor, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	// 跳过客户端
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client Attempting to Add Action. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(InstigatorActor)))
		{
			NewAction->StartAction(InstigatorActor);
		}
	}

}

// 移除行动
// 判断行动的正确性后进行移除
void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if (ensure(ActionToRemove))
	{
		Actions.Remove(ActionToRemove);
	}
}

// 启动行动
// 遍历所有可用行动，根据行动名称执行对应行动，并且根据标签类型判断是否可以执行
bool USActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->CanStart(InstigatorActor))
			{

				// 客户端
				if (!GetOwner()->HasAuthority())
				{
					ServerStartAction(InstigatorActor, ActionName);
				}
				Action->StartAction(InstigatorActor);
				return true;
			}
			else
			{
				FString DebugMsg = FString::Printf(TEXT("Failed To Run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			}
		}
	}
	return false;
}

void USActionComponent::ServerStartAction_Implementation(AActor* InstigatorActor, FName ActionName)
{
	StartActionByName(InstigatorActor, ActionName);
}

// 结束行动
// 遍历所有可用行动，根据行动名称结束对应行动，并且根据运行状态判断是否可以执行
bool USActionComponent::StopActionByName(AActor* InstigatorActor, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				// 客户端
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(InstigatorActor, ActionName);
				}

				Action->StopAction(InstigatorActor);
				return true;
			}
			else
			{
				FString DebugMsg = FString::Printf(TEXT("Failed To Stop: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			}
			
		}
	}
	return false;
}

void USActionComponent::ServerStopAction_Implementation(AActor* InstigatorActor, FName ActionName)
{
	StopActionByName(InstigatorActor, ActionName);
}

// 当有Action发生变化的时候请求复制
bool USActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (USAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

// 查看FGameplayTagContainer是否有特定的gameplaytag
bool USActionComponent::ActionCompHasTag(const FGameplayTag& TagToCheck)
{
	return ActiveGameplayTags.HasTag(TagToCheck);
}