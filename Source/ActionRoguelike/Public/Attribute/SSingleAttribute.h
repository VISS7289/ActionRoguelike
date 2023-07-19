// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SSingleAttribute.generated.h"

class USAttributeComponent;

// ע������ֵ�ı��¼�
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeValueChanged, AActor*, InstigatordActor, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USSingleAttribute : public UObject
{
	GENERATED_BODY()

public:

	USSingleAttribute();

	void InitValue(USAttributeComponent* AttributeComp);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Value; // ֵ

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float ValueMax; // ���ֵ

	UPROPERTY()
	USAttributeComponent* OwnerComp;

	UFUNCTION()
	virtual void ApplyBroadCast(AActor* InstigatordActor, float NewValue, float Delta);

public:

	// �Ƿ������
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsGreaterZero() const;

	// �Ƿ����ֵ
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsValueMax() const;

	// ֵ�ı�
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyValueChange(AActor* InstigatordActor, float Delta);
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float CanValueChange(AActor* InstigatordActor, float Delta);

	// ֵ��ȡ
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetValue();

	// ���ֵ��ȡ
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetValueMax();

	// ע��ֵ�ı��¼�
	UPROPERTY(BlueprintAssignable)
	FOnAttributeValueChanged ValueChanged;
	
};
