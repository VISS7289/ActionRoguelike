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

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp; // ���ɱ����

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp; // ��������

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp; // �������

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	USAttributeComponent* AttributeComp; // �������

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass; // ������

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim; // ��ͨ������̫��

	FTimerHandle TimerHandle_PrimaryAttack; // ��ʱ

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);


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

	// ��ͨ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryAttackAction;

	// ��������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryInteractAction;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// ��ͨ����
	void PrimaryAttack();
	// ��ʱ����
	void PrimaryAttack_TimeElapsed();

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
