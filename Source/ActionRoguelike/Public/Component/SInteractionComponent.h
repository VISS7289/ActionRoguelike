// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊ��ҽ������
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// 1�ν���
	void PrimaryInteract();

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:

	// ���ڼ��
	void FindBestInteractable();

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	AActor* FocusActor; // ��⵽�Ŀɽ�������

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance; // ��ⳤ��

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius; // ���뾶

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel; // ��ײ�����

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass; // Ĭ����ʾUI��

	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance; // Ĭ����ʾUIʵ��

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
