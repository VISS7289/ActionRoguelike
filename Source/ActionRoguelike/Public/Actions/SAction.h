// Fill out your copyright notice in the Description page of Project Settings.
// 该类为行动基类
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"


class UWorld;
class USActionComponent; // 行动组件

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(Replicated)
	TObjectPtr<USActionComponent> ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags; // 执行标签

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags; // 阻挡标签

	UPROPERTY(ReplicatedUsing="OnRep_IsRunning")
	int Running = 0; // 运行状态

	UFUNCTION()
	void OnRep_IsRunning();

public:

	void Initialize(USActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

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

	virtual UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const override
	{
		return true;
	}


};
