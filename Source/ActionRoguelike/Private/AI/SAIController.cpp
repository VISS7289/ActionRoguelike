// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();
	// ������Ϊ��
	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree Is A NullPtr! Please Set Behavior Tree In AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}


	//// ����
	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (MyPawn)
	//{
	//	// ����׷�ٵص���׷�ٶ���
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	//}

}