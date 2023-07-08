// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ�ɽ�����������
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGmeplayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent; // ��̬������
class UParticleSystemComponent; // ����ϵͳ

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGmeplayInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float TargetPitch = 110; // ��ʱ�ĸ�����

	UPROPERTY(EditAnywhere)
	FVector TargetTransform = FVector(0,0,0); // ��ʱ��λ��

	// �����ӿ�
	void Interact_Implementation(APawn* InstigatorPawn);

	// ���ؽӿ�
	void OnActorLoaded_Implementation();
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh; // ���ӵ���

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh; // ���Ӹ���

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Treasure; // ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp; // ����ϵͳ

	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", SaveGame)
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
