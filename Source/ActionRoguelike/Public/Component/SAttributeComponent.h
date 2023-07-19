// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ����������
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

// ע������ֵ�ı��¼�
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatordActor, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);

class USSingleAttribute;

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

	// ɱ������
	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatordActor);

	// Sets default values for this component's properties
	USAttributeComponent();

	UPROPERTY(EditAnywhere, Category = "Attributes")
	TSubclassOf<USSingleAttribute> EnergyClass;
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	USSingleAttribute* Energy;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Attributes")
	float Health; // ����ֵ

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float HealthMax; // �������ֵ

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatordActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRageChanged(AActor* InstigatordActor, float NewRage, float Delta);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Rage; // ŭ��ֵ

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float RageMax; // ���ŭ��ֵ

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
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth();

	// �������ֵ��ȡ
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthMax();

	// ŭ����ȡ
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRage() const;

	// ���ŭ����ȡ
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRageMax() const;

	// �ı�ŭ��
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRage(AActor* InstigatordActor, float Delta);

	virtual void BeginPlay();

	// ע������ֵ�ı��¼�
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	// ע��ŭ��ֵ�ı��¼�
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;
};
