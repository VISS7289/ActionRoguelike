// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTaskRangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/World.h"
#include "Component/SAttributeComponent.h"



USBTTaskRangeAttack::USBTTaskRangeAttack()
{
	MaxBulleSpread = 8.0f;
}



// 执行攻击
EBTNodeResult::Type USBTTaskRangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 获取AIController
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		// 获取AICharacter
		ACharacter* AICharacter = Cast<ACharacter>(MyController->GetPawn());
		if (AICharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		// 获取AI左手骨骼插槽
		FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation("Muzzle_01");
		// 获取攻击对象
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		// 攻击对象应该活着
		if (!USAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		// 设置子弹攻击朝向，指向目标现在所处位置
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotator = Direction.Rotation();
		// 添加一些扰动
		MuzzleRotator.Pitch += FMath::RandRange(0.0f, MaxBulleSpread);
		MuzzleRotator.Yaw += FMath::RandRange(-MaxBulleSpread, MaxBulleSpread);

		// 设置生成抛射物的碰撞信息与抛射物所有者信息
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = AICharacter;

		// 生成抛射物
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotator, Params);
		// 返回生成结果
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;;
}