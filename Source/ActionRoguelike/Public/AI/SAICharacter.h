// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊһ��AI
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"


class UPawnSensingComponent; // ������֪���
class USAttributeComponent; // �������


UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UPawnSensingComponent* PawnSensingComponent; // ������֪���

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USAttributeComponent* AttributeComp; // �������

	// ע���¼��ص�����
	virtual void PostInitializeComponents() override;

	// ���ù�������
	void SetTargetActor(AActor* NewTarget);

	// ע�⵽���ʱ����ӡ������Ϣ�����úڰ�ע�����
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	// ����ֵ�ı�ʱ
	UFUNCTION()
	void GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};