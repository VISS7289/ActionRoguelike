// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Math/Vector.h"

// 重载时刻节点
// 通过计算AI与目标的距离判断是否在攻击范围内
void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	// 获取黑板
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		// 获取目标
		AActor* TargetAcotr = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
		if (TargetAcotr)
		{
			// 获取AI控制器
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					// 计算玩家与AI的距离
					float Distance = FVector::Distance(TargetAcotr->GetActorLocation(), AIPawn->GetActorLocation());

					bool bWithinRange = Distance < 2000.0f;
					bool bHasLineSight = false;
					// 在攻击范围内判断是否在视线范围内
					if (bWithinRange)
					{
						bHasLineSight = MyController->LineOfSightTo(TargetAcotr);
					}
					// 修改黑板变量
					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLineSight);
				}
			}
		}
	}

}