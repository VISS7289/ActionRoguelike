// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
ASAICharacter::ASAICharacter()
{

	// ���û�����֪���
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// ע���¼��ص�����
void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
}

// ע�⵽���ʱ����ӡ������Ϣ�����úڰ�ע�����
void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	// ��ȡAI������
	AAIController* MyController = Cast<AAIController>(GetController());
	if (MyController)
	{
		// ��ȡ�ڰ�
		UBlackboardComponent* BBComp = MyController->GetBlackboardComponent();
		// ����Ŀ��
		BBComp->SetValueAsObject("TargetActor", Pawn);
		// ��ӡ
		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Green, 2.0f, true);
	}
}

