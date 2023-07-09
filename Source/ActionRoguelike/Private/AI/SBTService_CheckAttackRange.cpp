// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Math/Vector.h"

// ����ʱ�̽ڵ�
// ͨ������AI��Ŀ��ľ����ж��Ƿ��ڹ�����Χ��
void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	// ��ȡ�ڰ�
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		// ��ȡĿ��
		AActor* TargetAcotr = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
		if (TargetAcotr)
		{
			// ��ȡAI������
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					// ���������AI�ľ���
					float Distance = FVector::Distance(TargetAcotr->GetActorLocation(), AIPawn->GetActorLocation());

					bool bWithinRange = Distance < 2000.0f;
					bool bHasLineSight = false;
					// �ڹ�����Χ���ж��Ƿ������߷�Χ��
					if (bWithinRange)
					{
						bHasLineSight = MyController->LineOfSightTo(TargetAcotr);
					}
					// �޸ĺڰ����
					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLineSight);
				}
			}
		}
	}

}