// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();
	// 运行行为树
	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree Is A NullPtr! Please Set Behavior Tree In AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}


	//// 设置
	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//if (MyPawn)
	//{
	//	// 设置追踪地点与追踪对象
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	//}

}