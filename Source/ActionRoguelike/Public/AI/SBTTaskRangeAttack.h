// Fill out your copyright notice in the Description page of Project Settings.
// 该类为AI向玩家远程攻击Task
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTaskRangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTTaskRangeAttack : public UBTTaskNode
{
	GENERATED_BODY()

private:

	// 重载执行函数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass; // 发射物
	
};
