// Fill out your copyright notice in the Description page of Project Settings.
// 该类为可交互宝箱物体
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGmeplayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent; // 静态网格体
class UParticleSystemComponent; // 粒子系统

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGmeplayInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float TargetPitch = 110; // 打开时的俯仰角

	UPROPERTY(EditAnywhere)
	FVector TargetTransform = FVector(0,0,0); // 打开时的位移

	// 交互接口
	void Interact_Implementation(APawn* InstigatorPawn);

	// 加载接口
	void OnActorLoaded_Implementation();
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh; // 箱子底座

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh; // 箱子盖子

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Treasure; // 宝藏

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp; // 粒子系统

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
