// Fill out your copyright notice in the Description page of Project Settings.
// 该类为游戏模式，自动生成敌人AI
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"


class UEnvQuery; // 环境查询
class UEnvQueryInstanceBlueprintWrapper; // 环境查询
class UCurveFloat; // 浮点数曲线

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

	// 死亡处理
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	// 杀死所有AI
	UFUNCTION(Exec)
	void KillAll();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DiffcultyCurve; // 游戏难度--敌人AI数量

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery; // 环境查询

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimeInterval; // 重新生成时间

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass; // 生成的AI

	FTimerHandle TimerHandle_SpawnBots; // 生成AI计时器

	UFUNCTION()
	void SpawnBotTimerElapsed(); // 周期生成AI

	UFUNCTION()
	void RespawnPlayer(AController* Controller); // 重新生成玩家

	// EQS生成后生成AI
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

};
