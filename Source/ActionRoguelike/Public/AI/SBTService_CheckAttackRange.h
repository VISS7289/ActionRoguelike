// Fill out your copyright notice in the Description page of Project Settings.
// ����ʵ����AI�жϽ�ɫ�Ƿ���AI�Ĺ�����Χ��

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "SBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttackRangeKey; // ����ж��Ƿ�λ�ڹ�����Χ�ڵĺڰ����
	// ����ʱ�̽ڵ�
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
