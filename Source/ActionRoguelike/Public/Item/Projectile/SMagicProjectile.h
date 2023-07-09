// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊħ������������
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class USphereComponent; // ��
class UProjectileMovementComponent; // Ͷ�����ƶ����
class UParticleSystemComponent; // ����ϵͳ
class UParticleSystem; // ������Ч

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitEffect; // ������Ч

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp; // ��

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComp; // Ͷ�����ƶ����

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* EffectComp; // ����ϵͳ


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ע���¼��ص�����
	virtual void PostInitializeComponents() override;

	// �����ײ�¼�������  
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};
