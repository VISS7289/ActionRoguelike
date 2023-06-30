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

	// ����Actor�ϵ�USAttributeComponent
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	// �ж�Actor�Ƿ����
	UFUNCTION(BlueprintCallable, Category = "Attributes", meta=(DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* FromActor);

	// Sets default values for this component's properties
	USAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Health; // ����ֵ

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float HealthMax; // �������ֵ


public:	

	// �Ƿ����
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	// �Ƿ���Ѫ
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth() const;

	// ����ֵ�ı�
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatordActor, float Delta);

	// ����ֵ��ȡ
	float GetHealth();

	// �������ֵ��ȡ
	float GetHealthMax();

	// ע������ֵ�ı��¼�
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
};
