// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/Actor.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "Curves/CurveFloat.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"


ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	// 每隔一段时间就重新生成AI
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimeInterval, true);
}

// 杀死所有AI
void ASGameModeBase::KillAll()
{
	// 查询当前AI
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		// 如果有属性组件，调用属性组件的自杀
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); 
		}
	}
}

// 周期生成AI
void ASGameModeBase::SpawnBotTimerElapsed()
{

	// 限制生成数量
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		// 查询当前存货敌人AI数量
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i Alive Bots."), NrOfAliveBots);

	// 设置限制数量
	float MaxBotCount = 10.0f;
	// 如果存在难度曲线就根据难度曲线设置
	if (DiffcultyCurve)
	{
		MaxBotCount = DiffcultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At Maximum Bot Capacity. Skiping Bot Spawn."));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QureryInstance =  UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest25Pct, nullptr);
	if (ensure(QureryInstance))
	{
		QureryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}	
}

// EQS生成后生成AI
void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	// 确认EQS生成
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot Eqs Query Failed!"));
		return;
	}

	// 生成敌人AI
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	// 设置生成物规则
	FActorSpawnParameters SpawnParames;
	SpawnParames.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// 生成AI，对高度修正，以防卡地里
	if (Locations.IsValidIndex(0))
	{
		FVector DeltaH = FVector(0, 0, 100);
		AActor* NewBot = GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0]+ DeltaH, FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0] + DeltaH, 50.0f, 32, FColor::Green, false, 60.0f); // Debug球
	}
}