// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
ASAICharacter::ASAICharacter()
{

	// ���û�����֪���
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	// ����������
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	// ���û�����ʱ�Զ����AI������
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// ע���¼��ص�����
void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// �������ʱ����ע�����
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	// ����ֵ�ı�ʱ
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::GetHealthChange);
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

// ����ֵ�ı�ʱ
void ASAICharacter::GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// �ܻ��ж�
	if (Delta <= 0.0f)
	{
		
		// �����ж�
		if (NewHealth <= 0.0f)
		{
			// ֹͣ��Ϊ��
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			SetLifeSpan(10.0f);
		}
	}
}
