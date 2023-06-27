// Fill out your copyright notice in the Description page of Project Settings.
// 该类为一个AI
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"


class UPawnSensingComponent;


UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	// 注册事件回调函数
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UPawnSensingComponent* PawnSensingComponent; // 注意力组件

	// 注意到玩家时，打印调试信息与设置黑板注意对象
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

};
