// Fill out your copyright notice in the Description page of Project Settings.
// 该类为行动基类
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"


class UWorld;
class USActionComponent; // 行动组件
class USAction_Cold;


USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int Running;

	UPROPERTY()
	AActor* InstigatorActor;

};

USTRUCT()
struct FColdSet
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Cold")
	TSubclassOf<USAction_Cold> ColdClass;

	UPROPERTY(EditDefaultsOnly, Category = "Cold")
	float ColdTime;

};


/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Cold")
	FColdSet ColdSetting;
	UPROPERTY()
	USAction_Cold* ColdInstance;

	UPROPERTY(Replicated)
	TObjectPtr<USActionComponent> ActionComp;

	// 返回所在的ActionComponent
	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags; // 执行标签

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags; // 阻挡标签

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData; // 同步的数据
	//int Running = 0; // 运行状态

	UPROPERTY()
	AActor* Owner;

	// 同步数据时
	UFUNCTION()
	void OnRep_RepData();

public:

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void Initialize(USActionComponent* NewActionComp);

	// 判断是否可以开始
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* InstigatorActor);

	// 开始行动
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* InstigatorActor);

	// 结束行动
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* InstigatorActor);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	// 返回UWorld
	virtual UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const override
	{
		return true;
	}


};
