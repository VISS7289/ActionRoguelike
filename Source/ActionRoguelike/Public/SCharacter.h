// Fill out your copyright notice in the Description page of Project Settings.
// 该类为玩家角色类
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Engine/EngineTypes.h"
#include "SCharacter.generated.h"

class USpringArmComponent; // 弹簧臂组件
class UCameraComponent; // 摄像机组件
class USInteractionComponent; // 交互组件
class USAttributeComponent; // 属性组件

class UAnimMontage; // 动画蒙太奇

// 输入相关
class UInputMappingContext;
class UInputAction;
class UInputComponent;


UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp; // 弹簧臂组件

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp; // 摄像机组件

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp; // 交互组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	USAttributeComponent* AttributeComp; // 属性组件

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass; // 发射物

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim; // 普通攻击蒙太奇

	FTimerHandle TimerHandle_PrimaryAttack; // 延时

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);


// 输入相关
protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// 普通攻击
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryAttackAction;

	// 交互物体
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryInteractAction;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// 普通攻击
	void PrimaryAttack();
	// 延时攻击
	void PrimaryAttack_TimeElapsed();

	// 交互物体
	void PrimaryInteract();

	// 注册事件回调函数
	virtual void PostInitializeComponents() override;

	// 死亡判断
	UFUNCTION()
	void GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};
