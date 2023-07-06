// Fill out your copyright notice in the Description page of Project Settings.
// ������ըҩͰ
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class UStaticMeshComponent; // ��̬������
class URadialForceComponent; // ���������


UCLASS()
class ACTIONROGUELIKE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp; // ��̬������

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp; // ���������

	// ע���¼��ص�����
	virtual void PostInitializeComponents() override;

	UFUNCTION(NetMulticast, UnReliable)
	void MulticastExplode();

	// �����ײ�¼�������  
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
