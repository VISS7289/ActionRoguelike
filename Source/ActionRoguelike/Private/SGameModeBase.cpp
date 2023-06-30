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
	// ÿ��һ��ʱ�����������AI
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimeInterval, true);
}

// ɱ������AI
void ASGameModeBase::KillAll()
{
	// ��ѯ��ǰAI
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		// �����������������������������ɱ
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); 
		}
	}
}

// ��������AI
void ASGameModeBase::SpawnBotTimerElapsed()
{

	// ������������
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		// ��ѯ��ǰ�������AI����
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i Alive Bots."), NrOfAliveBots);

	// ������������
	float MaxBotCount = 10.0f;
	// ��������Ѷ����߾͸����Ѷ���������
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

// EQS���ɺ�����AI
void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	// ȷ��EQS����
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot Eqs Query Failed!"));
		return;
	}

	// ���ɵ���AI
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	// �������������
	FActorSpawnParameters SpawnParames;
	SpawnParames.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// ����AI���Ը߶��������Է�������
	if (Locations.IsValidIndex(0))
	{
		FVector DeltaH = FVector(0, 0, 100);
		AActor* NewBot = GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0]+ DeltaH, FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0] + DeltaH, 50.0f, 32, FColor::Green, false, 60.0f); // Debug��
	}
}