// Fill out your copyright notice in the Description page of Project Settings.
// 该类为一个AI
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"


class UPawnSensingComponent; // 环境感知组件
class USAttributeComponent; // 属性组件
class UUserWidget; // UI
class USWorldUserWidget; // AI血条UI
class USActionComponent; // 行动组件

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass; // 敌人血条UI

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass; // 敌人注意UI

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	FName TimeToHitParamName; // 受击闪烁材质参数名称

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UPawnSensingComponent* PawnSensingComponent; // 环境感知组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USAttributeComponent* AttributeComp; // 属性组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USActionComponent* ActionComp; // 行动组件

	// 注册事件回调函数
	virtual void PostInitializeComponents() override;

	// 设置攻击对象
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* NewTarget);

	// 获取当前攻击对象
	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;

	// 注意到玩家时
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	// 生命值改变时
	UFUNCTION()
	void GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
