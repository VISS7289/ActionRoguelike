// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ�ж����
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// ����ж�
	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddAction(TSubclassOf<USAction> ActionClass);

	// �����ж�
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* InstigatorActor, FName ActionName);

	// �����ж�
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* InstigatorActor, FName ActionName);

	// Sets default values for this component's properties
	USActionComponent();

protected:

	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<TSubclassOf<USAction>> DefaultActions;

	TArray<USAction*> Actions; // �����ж�

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
