// Fill out your copyright notice in the Description page of Project Settings.
// ����ΪAI������
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAIController.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASAIController : public AAIController
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree; // ��Ϊ��

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
