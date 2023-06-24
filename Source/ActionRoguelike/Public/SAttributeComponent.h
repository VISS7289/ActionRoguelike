// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ����������
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

// ע������ֵ�ı��¼�
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatordActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Health; // ����ֵ


public:	

	// ����ֵ�ı�
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);

	// ����ֵ��ȡ
	float GetHealth();

	// ע������ֵ�ı��¼�
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
};
