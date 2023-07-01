// Fill out your copyright notice in the Description page of Project Settings.
// 该类为判断AI是否血量过低
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()

public:

	USBTService_CheckHealth();

private:

	UPROPERTY(EditAnywhere, Category = "AI")
	float LowHealthFraction;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector LowHealthKey; // 添加判断是否血量较低的黑板变量

	// 重载时刻节点
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
