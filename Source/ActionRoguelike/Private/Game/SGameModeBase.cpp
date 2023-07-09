// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/Actor.h"
#include "AI/SAICharacter.h"
#include "Component/SAttributeComponent.h"
#include "Curves/CurveFloat.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "Player/SCharacter.h"
#include "Player/SPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Game/SSaveGame.h"
#include "GameFramework/GameStateBase.h"
#include "Game/SGmeplayInterface.h"
#include "Serialization/NameAsStringProxyArchive.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"



// �Ƿ����ɵ���AI���״���
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), false, TEXT("Enable Spawning Bots Via Timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval = 2.0f;
	CreditsPerKill = 100;

	PlayerStateClass = ASPlayerState::StaticClass();

	SlotName = "SaveGame01";
}

// LoadSaveGame��ʱ���е�Actor��InitGameû���س�����������StartPlay���Լ��أ�������
void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	//LoadSaveGame();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	// ÿ��һ��ʱ�����������AI
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimeInterval, true);

	LoadSaveGame();
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

// ������Ϸ
// Why? GameStateΪɶ����������Ϊɶ#include "GameFramework/GameStateBase.h"���У�
void ASGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // ����ֻ��һ�����
		}
	}

	CurrentSaveGame->SaveActors.Empty();

	// ���������е�����Actor
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// ֻ����ɽ�������
		if (!Actor->Implements<USGmeplayInterface>())
		{
			continue;
		}
		// ����Actor��������λ��
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();
		// ����״̬
		FMemoryWriter MemWriter(ActorData.ByteData);
		FNameAsStringProxyArchive Ar(MemWriter);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);
		// ����
		CurrentSaveGame->SaveActors.Add(ActorData);

	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

// ������Ϸ
void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0)) // ���ڱ������Ϸ
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed To Load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Load SaveGame Data."));


		// ���������е�����Actor
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// ֻ����ɽ�������
			if (!Actor->Implements<USGmeplayInterface>())
			{
				continue;
			}

			// �ɽ��������transform�ñ����transform
			for (FActorSaveData ActorData : CurrentSaveGame->SaveActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);
					// ����״̬��Ϣ
					FMemoryReader MemReader(ActorData.ByteData);
					FNameAsStringProxyArchive Ar(MemReader);
					Ar.ArIsSaveGame = true;
					Actor->Serialize(Ar);

					ISGmeplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}

		}

	}
	else 
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Create New SaveGame Data."));
	}
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	///////////////////////////////////////////////////////////////////
	CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (CurrentSaveGame == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed To Load SaveGame Data."));
		return;
	}
	///////////////////////////////////////////////////////////////////


	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
}