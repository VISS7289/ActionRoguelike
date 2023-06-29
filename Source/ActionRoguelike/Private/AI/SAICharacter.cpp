// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
ASAICharacter::ASAICharacter()
{

	// 设置环境感知组件
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// 注册事件回调函数
void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
}

// 注意到玩家时，打印调试信息与设置黑板注意对象
void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	// 获取AI控制器
	AAIController* MyController = Cast<AAIController>(GetController());
	if (MyController)
	{
		// 获取黑板
		UBlackboardComponent* BBComp = MyController->GetBlackboardComponent();
		// 设置目标
		BBComp->SetValueAsObject("TargetActor", Pawn);
		// 打印
		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Green, 2.0f, true);
	}
}

