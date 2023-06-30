// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/SWorldUserWidget.h"


// Sets default values
ASAICharacter::ASAICharacter()
{

	// 设置环境感知组件
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	// 添加属性组件
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	// 放置或生成时自动添加AI控制器
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 受击闪烁材质参数名称
	TimeToHitParamName = "TimeToHit";

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
	// 将注意到的对象设为攻击对象
	SetTargetActor(Pawn);
	// 打印
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Green, 2.0f, true);
}

// 生命值改变时
void ASAICharacter::GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// 受击判断
	if (Delta <= 0.0f)
	{

		// 材质闪烁（已经设置好了，只需要TimeToHit与游戏时间相同，就会闪烁1s）
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		// 添加UI（为什么不在begin play做）
		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			ActiveHealthBar->AttacheActor = this;
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AddToViewport();
			}
		}
		


		// 攻击对象是自己的话不能将自己设为攻击目标
		if (InstigatordActor != this)
		{
			SetTargetActor(InstigatordActor);
		}

		// 死亡判断
		if (NewHealth <= 0.0f)
		{
			// 停止行为树
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			// 启用物理模拟（所有关节自由落体并相互之间模拟物理，达成死亡自动倒下的效果）
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			// 10s后销毁自身
			SetLifeSpan(10.0f);
		}
	}
}

// 设置攻击对象
void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	// 获取AI控制器
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		// 设置目标
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);

	}
}