// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ�ж�AI�Ƿ�Ѫ������
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
	FBlackboardKeySelector LowHealthKey; // ����ж��Ƿ�Ѫ���ϵ͵ĺڰ����

	// ����ʱ�̽ڵ�
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
