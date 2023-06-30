// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/SWorldUserWidget.h"


// Sets default values
ASAICharacter::ASAICharacter()
{

	// ���û�����֪���
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	// ����������
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	// ���û�����ʱ�Զ����AI������
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// �ܻ���˸���ʲ�������
	TimeToHitParamName = "TimeToHit";

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
	// ��ע�⵽�Ķ�����Ϊ��������
	SetTargetActor(Pawn);
	// ��ӡ
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Green, 2.0f, true);
}

// ����ֵ�ı�ʱ
void ASAICharacter::GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// �ܻ��ж�
	if (Delta <= 0.0f)
	{

		// ������˸���Ѿ����ú��ˣ�ֻ��ҪTimeToHit����Ϸʱ����ͬ���ͻ���˸1s��
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		// ���UI��Ϊʲô����begin play����
		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			ActiveHealthBar->AttacheActor = this;
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AddToViewport();
			}
		}
		


		// �����������Լ��Ļ����ܽ��Լ���Ϊ����Ŀ��
		if (InstigatordActor != this)
		{
			SetTargetActor(InstigatordActor);
		}

		// �����ж�
		if (NewHealth <= 0.0f)
		{
			// ֹͣ��Ϊ��
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			// ��������ģ�⣨���йؽ��������岢�໥֮��ģ��������������Զ����µ�Ч����
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			// 10s����������
			SetLifeSpan(10.0f);
		}
	}
}

// ���ù�������
void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	// ��ȡAI������
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		// ����Ŀ��
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);

	}
}