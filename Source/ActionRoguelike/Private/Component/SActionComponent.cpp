// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SActionComponent.h"
#include "Actions/SAction.h"


// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
	
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

// 添加行动
// 根据类引用来添加具体的行动
void USActionComponent::AddAction(AActor* InstigatorActor, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
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