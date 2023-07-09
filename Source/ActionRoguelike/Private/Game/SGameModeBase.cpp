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



// 是否生成敌人AI作弊代码
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), false, TEXT("Enable Spawning Bots Via Timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval = 2.0f;
	CreditsPerKill = 100;

	PlayerStateClass = ASPlayerState::StaticClass();

	SlotName = "SaveGame01";
}

// LoadSaveGame的时候有的Actor在InitGame没加载出来，但是在StartPlay可以加载？？？？
void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	//LoadSaveGame();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	// 每隔一段时间就重新生成AI
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimeInterval, true);

	LoadSaveGame();
}

// 死亡处理
// 如果死亡的是玩家，则在2s后复活。如果死亡的是AI，则增加积分
void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player) // 死的是玩家
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayer", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn) // 死的是AI
	{
		ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditsPerKill);

			UE_LOG(LogTemp, Log, TEXT("NewCredits：%d"), PS->GetCredits());
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled：VictimActor：%s，Killer：%s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

// 重新生成玩家
void ASGameModeBase::RespawnPlayer(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
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

	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawn Disabled via 'CVarSpawnBots'."));
		return;
	}

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

// 保存游戏
// Why? GameState为啥不用声明？为啥#include "GameFramework/GameStateBase.h"就行？
void ASGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // 现在只有一个玩家
		}
	}

	CurrentSaveGame->SaveActors.Empty();

	// 遍历世界中的所有Actor
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// 只处理可交互物体
		if (!Actor->Implements<USGmeplayInterface>())
		{
			continue;
		}
		// 保存Actor的名称与位置
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();
		// 保存状态
		FMemoryWriter MemWriter(ActorData.ByteData);
		FNameAsStringProxyArchive Ar(MemWriter);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);
		// 保存
		CurrentSaveGame->SaveActors.Add(ActorData);

	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

// 加载游戏
void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0)) // 存在保存的游戏
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed To Load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Load SaveGame Data."));


		// 遍历世界中的所有Actor
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// 只处理可交互物体
			if (!Actor->Implements<USGmeplayInterface>())
			{
				continue;
			}

			// 可交互物体的transform用保存的transform
			for (FActorSaveData ActorData : CurrentSaveGame->SaveActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);
					// 设置状态信息
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