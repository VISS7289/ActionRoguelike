// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ�ж�����
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"


class UWorld;
class USActionComponent; // �ж����
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

	// �������ڵ�ActionComponent
	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags; // ִ�б�ǩ

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags; // �赲��ǩ

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData; // ͬ��������
	//int Running = 0; // ����״̬

	UPROPERTY()
	AActor* Owner;

	// ͬ������ʱ
	UFUNCTION()
	void OnRep_RepData();

public:

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void Initialize(USActionComponent* NewActionComp);

	// �ж��Ƿ���Կ�ʼ
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* InstigatorActor);

	// ��ʼ�ж�
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* InstigatorActor);

	// �����ж�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* InstigatorActor);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	// ����UWorld
	virtual UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const override
	{
		return true;
	}


};
