// Fill out your copyright notice in the Description page of Project Settings.
// ������ǻ�����˸����
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STargetDummy.generated.h"

class USAttributeComponent; // ����������
class UStaticMeshComponent; // ��̬������

UCLASS()
class ACTIONROGUELIKE_API ASTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere)
	USAttributeComponent* AttributeComp; // ����������

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp; // ��̬������

	// �˺�ʱ��˸
	UFUNCTION()
	void GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};