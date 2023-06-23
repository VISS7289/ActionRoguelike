// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ��Ϸ�ӿ���
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGmeplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGmeplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONROGUELIKE_API ISGmeplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// �����ӿ�
	UFUNCTION(BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
};