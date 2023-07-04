// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/Actor.h"
#include "AI/SAICharacter.h"
#include "Component/SAttributeComponent.h"
#include "Curves/CurveFloat.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "SPlayerState.h"

// �Ƿ����ɵ���AI���״���
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable Spawning Bots Via Timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval = 2.0f;
	CreditsPerKill = 100;

	PlayerStateClass = ASPlayerState::StaticClass();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	// ÿ��һ��ʱ�����������AI
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimeInterval, true);
}

// ��������
// �������������ң�����2s�󸴻�����������AI�������ӻ���
void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player) // ���������
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayer", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn) // ������AI
	{
		ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditsPerKill);

			UE_LOG(LogTemp, Log, TEXT("NewCredits��%d"), PS->GetCredits());
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled��VictimActor��%s��Killer��%s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

// �����������
void ASGameModeBase::RespawnPlayer(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
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

	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawn Disabled via 'CVarSpawnBots'."));
		return;
	}

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