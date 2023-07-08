// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SValueChangedWidget.generated.h"

class USAttributeComponent; // �������
class UWidgetAnimation; // UI����
class UMaterialInstanceDynamic; // ��̬���ʽӿ�

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USValueChangedWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	USValueChangedWidget(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	FText TValue; // ֵText��ʽ

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	float Value; // ֵ

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	float MaxValue; // ���ֵ

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	float ValueRate; // ֵ�ٷֱ�

	UPROPERTY(BlueprintReadWrite, Category = "Player Health")
	UMaterialInstanceDynamic* ValueMaterial; // ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UWidgetAnimation* PauseValueAnim; // ֵ�仯����

protected:

	// UI�������ʱ������¼�
	virtual void NativeConstruct() override;

	// ����ֵ�ı�ʱ����UI
	UFUNCTION(BlueprintCallable)
	void GetValueChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewValue, float Delta);

	// ��Ҹı�ʱ����UI
	UFUNCTION(BlueprintCallable)
	void GetPlayerChange(APawn* InPawn);

	// ��ͨ��������ֵ����
	bool ValueChangeCommon(float NewValue);

	// ֵ��ʼ������
	virtual void ValueInit(USAttributeComponent* AttributeComp);

	// ���¼�����
	virtual void BindWithAttributeChanged(USAttributeComponent* AttributeComp);
	
};
