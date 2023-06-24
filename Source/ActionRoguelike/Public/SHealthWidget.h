// Fill out your copyright notice in the Description page of Project Settings.
// ����������Ѫ��
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHealthWidget.generated.h"

class USAttributeComponent;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	USHealthWidget(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	FText THealth; // ����ֵ

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	float Health; // ����ֵ

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	float MaxHealth; // �������ֵ

	UPROPERTY(BlueprintReadOnly, Category = "Player Health")
	float HealthRate; // ����ֵ�ٷֱ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UWidgetAnimation* PauseHealthAnim; // ����ֵ���ٶ���
	
protected:

	// UI�������ʱ������¼�
	virtual void NativeConstruct() override;

	// ����ֵ�ı�ʱ����UI
	UFUNCTION()
	void GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	// ��ͨ��������ֵ����
	bool HealthChangeCommon(float NewHealth);
	
};
