// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SValueChangedWidget.h"
#include "SRageChangedWidget.generated.h"


class USAttributeComponent; // �������

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USRageChangedWidget : public USValueChangedWidget
{
	GENERATED_BODY()

public:

	USRageChangedWidget(const FObjectInitializer& ObjectInitializer);

protected:

	// ���¼�����
	virtual void BindWithAttributeChanged(USAttributeComponent* AttributeComp);

	// ֵ��ʼ������
	virtual void ValueInit(USAttributeComponent* AttributeComp);
	
};
