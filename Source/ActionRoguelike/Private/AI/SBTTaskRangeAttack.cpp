// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTaskRangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/World.h"




EBTNodeResult::Type USBTTaskRangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// ��ȡAIController
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		// ��ȡAICharacter
		ACharacter* AICharacter = Cast<ACharacter>(MyController->GetPawn());
		if (AICharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		// ��ȡAI���ֹ������
		FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation("Muzzle_01");
		// ��ȡ��������
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		// �����ӵ���������ָ��Ŀ����������λ��
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotator = Direction.Rotation();
		// �����������������ײ��Ϣ����������������Ϣ
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = AICharacter;
		// ����������
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotator, Params);
		// �������ɽ��
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;;
}