// Fill out your copyright notice in the Description page of Project Settings.
// ����Ϊһ��AI
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"


class UPawnSensingComponent; // ������֪���
class USAttributeComponent; // �������
class UUserWidget; // UI
class USWorldUserWidget; // AIѪ��UI
class USActionComponent; // �ж����

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass; // ����Ѫ��UI

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass; // ����ע��UI

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	FName TimeToHitParamName; // �ܻ���˸���ʲ�������

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UPawnSensingComponent* PawnSensingComponent; // ������֪���

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USAttributeComponent* AttributeComp; // �������

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USActionComponent* ActionComp; // �ж����

	// ע���¼��ص�����
	virtual void PostInitializeComponents() override;

	// ���ù�������
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* NewTarget);

	// ��ȡ��ǰ��������
	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;

	// ע�⵽���ʱ
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	// ����ֵ�ı�ʱ
	UFUNCTION()
	void GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

};
