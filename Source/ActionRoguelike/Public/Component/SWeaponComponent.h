// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Queue.h"
#include "SWeaponComponent.generated.h"

class UStaticMeshComponent; // ��̬������
class ASProjectileBase; // �ӵ�����

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USWeaponComponent();

	// ����Actor�ϵ�USWeaponComponent
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	static USWeaponComponent* GetWeaponComp(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	TSubclassOf<AActor> Fire(); // ����

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AddBullet(TSubclassOf<AActor> ProjectileClass); // װ��

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool CanFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void BulletTypeRight(); // ������ת

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void BulletTypeLeft(); // ������ת

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh; // ǹе������

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AActor> DefaultBullet; // Ĭ���ӵ�

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 BulletNumMax; // �ӵ�����

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TArray<TSubclassOf<AActor>> BulletInGun; // ǹе���ִ��ӵ�

	int32 NowBulletType;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<TSubclassOf<AActor>> BulletType;

	UFUNCTION(BlueprintCallable)
	void AddAll(); // װ��

	TSubclassOf<AActor> BulletPop();

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
