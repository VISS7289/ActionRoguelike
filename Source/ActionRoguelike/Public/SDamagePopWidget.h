// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SDamagePopWidget.generated.h"


class UCanvasPanel; // 画布
class UTextBlock; // 文字组件

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USDamagePopWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	USDamagePopWidget(const FObjectInitializer& ObjectInitializer);

protected:

	
};
