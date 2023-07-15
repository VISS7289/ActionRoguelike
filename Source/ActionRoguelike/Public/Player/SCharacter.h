// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ��ҽ�ɫ��
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Engine/EngineTypes.h"
#include "SCharacter.generated.h"

class USpringArmComponent; // ���ɱ����
class UCameraComponent; // ��������
class USInteractionComponent; // �������
class USAttributeComponent; // �������
class USActionComponent; // �ж����
class USWeaponComponent; // �������

class UAnimMontage; // ������̫��

// �������
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

	// ��������Ŀ���̨����
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.0f);

	// ��ȡ���λ������ת
	virtual FVector GetPawnViewLocation() const override;

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp; // ���ɱ����

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp; // ��������

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp; // �������

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	USAttributeComponent* AttributeComp; // �������

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USActionComponent* ActionComp; // �ж����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USWeaponComponent* WeaponComp; // �������

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


// �������
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

	// ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryParryAction;

	// ��ͨ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryAttackAction;

	// ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryDashAction;

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimarySprintDown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimarySprintRelease;

	// ��ɱ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryMustKillAction;

	// ��������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryInteractAction;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// ��
	void PrimaryParry();

	// ��ͨ����
	void PrimaryAttack();

	// ���
	void PrimaryDash();

	// ���ٿ�ʼ
	void PrimarySprintStart();
	// ���ٽ���
	void PrimarySprintEnd();

	// ��ɱ
	void PrimaryMustKill();

	// ��������
	void PrimaryInteract();

	// ע���¼��ص�����
	virtual void PostInitializeComponents() override;

	// �����ж�
	UFUNCTION()
	void GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};
