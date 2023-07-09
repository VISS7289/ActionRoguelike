// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ���˼�
#pragma once

#include "CoreMinimal.h"
#include "Actions/SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"


class USAttributeComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;  // ���˰ٷֱ�

	// ����ֵ�ı�ʱ����
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

public:

	// Action��ʼʱ����������ֵ�ı��¼�
	virtual void StartAction_Implementation(AActor* Instigator) override;

	// Action����ʱ��ȡ����������ֵ�ı��¼�
	virtual void StopAction_Implementation(AActor* Instigator) override;

	USActionEffect_Thorns();


};
