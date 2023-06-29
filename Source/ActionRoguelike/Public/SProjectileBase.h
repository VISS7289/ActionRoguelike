// Fill out your copyright notice in the Description page of Project Settings.
// 这个类提供了一个基础的抛射物
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent; // 球
class UProjectileMovementComponent; // 投射物移动组件
class UParticleSystemComponent; // 粒子系统
class UParticleSystem; // 粒子特效
class UAudioComponent; // 音效组件

UCLASS()
class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitEffect; // 粒子特效

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp; // 球

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComp; // 音效

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComp; // 投射物移动组件

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EffectComp; // 粒子系统

	// 自我销毁计时器
	FTimerHandle FTimerHandle_DestoryPrjDelay;
	float DestoryPrjDelay;

	UPROPERTY(EditAnywhere)
		float Damage; // 伤害

	// 自我销毁
	virtual void DestoryPrj();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 注册事件回调函数
	virtual void PostInitializeComponents() override;

	// 组件碰撞事件处理函数  
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
