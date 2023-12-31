// Fill out your copyright notice in the Description page of Project Settings.
// 该类为玩家交互组件
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// 1次交互
	void PrimaryInteract();

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:

	// 服务器交互
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	// 周期检测
	void FindBestInteractable();

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	AActor* FocusActor; // 检测到的可交互对象

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance; // 检测长度

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius; // 检测半径

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel; // 碰撞检测类

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass; // 默认提示UI类

	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance; // 默认提示UI实体

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
