// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ��Ϸģʽ���Զ����ɵ���AI
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"


class UEnvQuery; // ������ѯ
class UEnvQueryInstanceBlueprintWrapper; // ������ѯ
class UCurveFloat; // ����������

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ASGameModeBase();

	virtual void StartPlay() override;

	// ��������
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	// ɱ������AI
	UFUNCTION(Exec)
	void KillAll();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DiffcultyCurve; // ��Ϸ�Ѷ�--����AI����

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery; // ������ѯ

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimeInterval; // ��������ʱ��

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass; // ���ɵ�AI

	FTimerHandle TimerHandle_SpawnBots; // ����AI��ʱ��

	UFUNCTION()
	void SpawnBotTimerElapsed(); // ��������AI

	UFUNCTION()
	void RespawnPlayer(AController* Controller); // �����������

	// EQS���ɺ�����AI
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

};
