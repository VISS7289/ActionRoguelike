// Fill out your copyright notice in the Description page of Project Settings.
// 该类为玩家角色类
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagContainer.h"
#include "Component/SActionComponent.h"
#include "SCharacter.generated.h"

class USpringArmComponent; // 弹簧臂组件
class UCameraComponent; // 摄像机组件
class USInteractionComponent; // 交互组件
class USAttributeComponent; // 属性组件
class USActionComponent; // 行动组件
class USWeaponComponent; // 武器组件

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

	// 治疗自身的控制台命令
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.0f);

	// 获取相机位置与旋转
	virtual FVector GetPawnViewLocation() const override;

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp; // 弹簧臂组件

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp; // 摄像机组件

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp; // 交互组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	USAttributeComponent* AttributeComp; // 属性组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USActionComponent* ActionComp; // 行动组件
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer MoveBlockedTags; // 阻挡标签

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USWeaponComponent* WeaponComp; // 武器组件

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


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
	FVector PlayerMoveInput;
	bool Moving;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// 格挡
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryParryAction;

	// 普通攻击
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryAttackActionEnd;

	// 冲刺
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryDashAction;

	// 加速
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimarySprintDown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimarySprintRelease;

	// 蓄力
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AccumulateDown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AccumulateRelease;

	// 子弹种类左右切换
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BulletTypeRightAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BulletTypeLeftAction;

	// 必杀
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryMustKillAction;

	// 交互物体
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryInteractAction;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// 格挡
	void PrimaryParry();

	// 普通攻击
	void PrimaryAttack();
	void PrimaryAttackEnd();

	// 冲刺
	void PrimaryDash();

	// 跳跃开始
	void PrimaryJumpStart();
	// 跳跃结束
	void PrimaryJumpEnd();

	// 加速开始
	void PrimarySprintStart();
	// 加速结束
	void PrimarySprintEnd();

	// 蓄力开始
	void AccumulateStart();
	// 蓄力结束
	void AccumulateEnd();

	// 子弹向右切换
	void BulletTypeRight();
	// 子弹向左切换
	void BulletTypeLeft();

	// 必杀
	void PrimaryMustKill();

	// 交互物体
	void PrimaryInteract();

	// 注册事件回调函数
	virtual void PostInitializeComponents() override;

	// 死亡判断
	UFUNCTION()
	void GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

public:

	FVector GetPlayerMoveInput();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};
