// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "BrainComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/SWorldUserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/SAttributeComponent.h"
#include "Component/SActionComponent.h"
#include "Component/SWeaknessComponent.h"


// Sets default values
ASAICharacter::ASAICharacter()
{

	// 设置环境感知组件
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	// 添加属性组件
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	// 添加行动组件
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	// 添加弱点组件
	WeaknessComp = CreateDefaultSubobject<USWeaknessComponent>("WeaknessComp");

	// 放置或生成时自动添加AI控制器
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 受击闪烁材质参数名称
	TimeToHitParamName = "TimeToHit";

}

// 监听注意到对象与生命改变
void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::GetHealthChange);
}

// 注意到玩家时
// 打印调试信息与设置黑板注意对象，并显示提示UI
void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	if (GetTargetActor() != Pawn)
	{
		// 将注意到的对象设为攻击对象
		SetTargetActor(Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Green, 2.0f, true);
		USWorldUserWidget* NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedWidgetClass);
		if (NewWidget)
		{
			NewWidget->AttacheActor = this;
			// 让注意提示在血条上方
			NewWidget->AddToViewport(10);
		}
	}

}

// 生命值改变时
// 如果受到攻击，执行材质受击效果，切换攻击目标与死亡判定
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
			// 取消胶囊体碰撞
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			// 销毁弱点
			WeaknessComp->DestoryAllWeakness();
			// 10s后销毁自身
			SetLifeSpan(10.0f);
		}
	}
}

// 设置攻击对象
// 将黑板中的TargetActor设置为NewTarget
void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);

	}
}

// 获取当前攻击对象
AActor* ASAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	}

	return nullptr;
}