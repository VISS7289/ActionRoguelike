// Fill out your copyright notice in the Description page of Project Settings.
// ������ṩ��һ��������������
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent; // ��
class UProjectileMovementComponent; // Ͷ�����ƶ����
class UParticleSystemComponent; // ����ϵͳ
class UParticleSystem; // ������Ч
class UAudioComponent; // ��Ч���

UCLASS()
class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitEffect; // ������Ч

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp; // ��

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComp; // ��Ч

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComp; // Ͷ�����ƶ����

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EffectComp; // ����ϵͳ

	// �������ټ�ʱ��
	FTimerHandle FTimerHandle_DestoryPrjDelay;
	float DestoryPrjDelay;

	UPROPERTY(EditAnywhere)
		float Damage; // �˺�

	// ��������
	virtual void DestoryPrj();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ע���¼��ص�����
	virtual void PostInitializeComponents() override;

	// �����ײ�¼�������  
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
