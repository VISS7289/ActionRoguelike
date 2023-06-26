// Fill out your copyright notice in the Description page of Project Settings.
// ����ΪAI�����Զ�̹���Task
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

	// ����ִ�к���
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass; // ������
	
};
