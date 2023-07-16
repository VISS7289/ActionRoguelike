// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "BrainComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/SWorldUserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/SAttributeComponent.h"
#include "Component/SActionComponent.h"
#include "Component/SWeaknessComponent.h"


// Sets default values
ASAICharacter::ASAICharacter()
{

	// ���û�����֪���
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	// ����������
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	// ����ж����
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	// ����������
	WeaknessComp = CreateDefaultSubobject<USWeaknessComponent>("WeaknessComp");

	// ���û�����ʱ�Զ����AI������
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// �ܻ���˸���ʲ�������
	TimeToHitParamName = "TimeToHit";

}

// ����ע�⵽�����������ı�
void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::GetHealthChange);
}

// ע�⵽���ʱ
// ��ӡ������Ϣ�����úڰ�ע����󣬲���ʾ��ʾUI
void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	if (GetTargetActor() != Pawn)
	{
		// ��ע�⵽�Ķ�����Ϊ��������
		SetTargetActor(Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Green, 2.0f, true);
		USWorldUserWidget* NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedWidgetClass);
		if (NewWidget)
		{
			NewWidget->AttacheActor = this;
			// ��ע����ʾ��Ѫ���Ϸ�
			NewWidget->AddToViewport(10);
		}
	}

}

// ����ֵ�ı�ʱ
// ����ܵ�������ִ�в����ܻ�Ч�����л�����Ŀ���������ж�
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
			// ȡ����������ײ
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			// ��������
			WeaknessComp->DestoryAllWeakness();
			// 10s����������
			SetLifeSpan(10.0f);
		}
	}
}

// ���ù�������
// ���ڰ��е�TargetActor����ΪNewTarget
void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);

	}
}

// ��ȡ��ǰ��������
AActor* ASAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	}

	return nullptr;
}