// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ�ڶ��ӵ�
#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SBlackHoleProjectile.generated.h"


class URadialForceComponent; // ���������
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASBlackHoleProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASBlackHoleProjectile();

protected:

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* ForceComp; // ���������

	// ע���¼��ص�����
	virtual void PostInitializeComponents() override;

	// �����ײ�¼�������  
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
