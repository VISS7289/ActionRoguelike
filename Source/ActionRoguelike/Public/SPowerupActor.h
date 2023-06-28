// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ��Ѫ��Ʒ����
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGmeplayInterface.h"
#include "SPowerupActor.generated.h"


class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPowerupActor : public AActor, public ISGmeplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

	// �����ӿ�
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> SphereComp; // ��ײ��

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp; // ��̬������

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime; // ��������ʱ��

	FTimerHandle TimerHandle_RespawnTimer; // ��ʱ��

	// ��ʾҩƿ
	UFUNCTION()
	void ShowPowerup();

	// ����ҩƿ״̬
	void SetPowerupState(bool bNewIsActive);

	// ����ҩƿ��������ȴ
	void HideAndCooldownPowerup();


};
