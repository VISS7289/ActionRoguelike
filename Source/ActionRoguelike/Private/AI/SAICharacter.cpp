// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
ASAICharacter::ASAICharacter()
{

	// 设置环境感知组件
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	// 添加属性组件
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	// 放置或生成时自动添加AI控制器
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// 注册事件回调函数
void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// 看到玩家时设置注意对象
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	// 生命值改变时
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::GetHealthChange);
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

// 生命值改变时
void ASAICharacter::GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// 受击判断
	if (Delta <= 0.0f)
	{
		
		// 死亡判断
		if (NewHealth <= 0.0f)
		{
			// 停止行为树
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			SetLifeSpan(10.0f);
		}
	}
}
