// Fill out your copyright notice in the Description page of Project Settings.
// 该类为行动组件
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// 添加行动
	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddAction(TSubclassOf<USAction> ActionClass);

	// 启动行动
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* InstigatorActor, FName ActionName);

	// 结束行动
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* InstigatorActor, FName ActionName);

	// Sets default values for this component's properties
	USActionComponent();

protected:

	TArray<USAction*> Actions; // 所有行动

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
