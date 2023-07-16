// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SWeaknessComponent.generated.h"

class ASWeakness;
class USAttributeComponent; // 玩家属性组件

USTRUCT()
struct FSingleWeakness
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASWeakness> WeaknessActor;

	UPROPERTY(EditDefaultsOnly)
	FName WeaknessName;

	UPROPERTY(EditDefaultsOnly)
	FVector WeaknessScale;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USWeaknessComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USWeaknessComponent();
	void DestoryAllWeakness();

protected:

	UPROPERTY(EditAnywhere, Category = "Weakness")
	TArray<FSingleWeakness> Weaknesses; // 编辑器中设置的弱点

	UPROPERTY()
	TArray<ASWeakness*> AllWeakness; // 所有弱点

	UPROPERTY()
	USAttributeComponent* OwnerAttributeComp;

	UPROPERTY()
	ACharacter* OwningCharacter;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
