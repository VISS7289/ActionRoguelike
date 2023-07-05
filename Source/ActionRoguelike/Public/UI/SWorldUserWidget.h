// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ����Ѫ����
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"


class USizeBox;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset; // Ѫ��ƫ��

	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox; // �������

	// ÿ֡��������Ѫ��λ��
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (ExposeOnSpawn = true))
	AActor* AttacheActor; // ������
	
};
