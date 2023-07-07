// Fill out your copyright notice in the Description page of Project Settings.
// 该类为行动组件
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// 启动的标签
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	// 添加行动
	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddAction(AActor* InstigatorActor, TSubclassOf<USAction> ActionClass);

	// 移除行动
	UFUNCTION(BlueprintCallable, Category = "Action")
	void RemoveAction(USAction* ActionToRemove);

	// 启动行动
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* InstigatorActor, FName ActionName);

	// 结束行动
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* InstigatorActor, FName ActionName);

	// Sets default values for this component's properties
	USActionComponent();

protected:

	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* InstigatorActor, FName ActionName);

	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<TSubclassOf<USAction>> DefaultActions;

	UPROPERTY(Replicated)
	TArray<USAction*> Actions; // 所有行动

	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
