// Fill out your copyright notice in the Description page of Project Settings.
// 该类为加积分类
#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerupActor_Credits.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPowerupActor_Credits : public ASPowerupActor
{
	GENERATED_BODY()

public:

	ASPowerupActor_Credits();

	// 交互接口
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

private:

	UPROPERTY(EditAnywhere)
	int32 CreditsAmount; // 增加积分量
	
};
