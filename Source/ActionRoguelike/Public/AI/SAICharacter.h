// Fill out your copyright notice in the Description page of Project Settings.
// 该类为一个AI
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"


class UPawnSensingComponent; // 环境感知组件
class USAttributeComponent; // 属性组件


UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	FName TimeToHitParamName; // 受击闪烁材质参数名称

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UPawnSensingComponent* PawnSensingComponent; // 环境感知组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USAttributeComponent* AttributeComp; // 属性组件

	// 注册事件回调函数
	virtual void PostInitializeComponents() override;

	// 设置攻击对象
	void SetTargetActor(AActor* NewTarget);

	// 注意到玩家时，打印调试信息与设置黑板注意对象
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	// 生命值改变时
	UFUNCTION()
	void GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
