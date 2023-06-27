// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊһ��AI
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"


class UPawnSensingComponent;


UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	// ע���¼��ص�����
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UPawnSensingComponent* PawnSensingComponent; // ע�������

	// ע�⵽���ʱ����ӡ������Ϣ�����úڰ�ע�����
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

};
