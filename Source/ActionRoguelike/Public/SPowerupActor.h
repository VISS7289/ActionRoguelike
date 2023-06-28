// Fill out your copyright notice in the Description page of Project Settings.
// 该类为回血物品基类
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGmeplayInterface.h"
#include "SPowerupActor.generated.h"


class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPowerupActor : public AActor, public ISGmeplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

	// 交互接口
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> SphereComp; // 碰撞球

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp; // 静态网格体

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime; // 重新生成时间

	FTimerHandle TimerHandle_RespawnTimer; // 计时器

	// 显示药瓶
	UFUNCTION()
	void ShowPowerup();

	// 设置药瓶状态
	void SetPowerupState(bool bNewIsActive);

	// 隐藏药瓶并设置冷却
	void HideAndCooldownPowerup();


};
