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
	TSubclassOf<ASProjectileBase> Fire(); // ����

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AddBullet(TSubclassOf<ASProjectileBase> ProjectileClass); // װ��

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool CanFire();

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void BulletTypeRight(); // ������ת

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void BulletTypeLeft(); // ������ת

	TSubclassOf<ASProjectileBase> GetDefaultBullet();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh; // ǹе������

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<ASProjectileBase> DefaultBullet; // Ĭ���ӵ�

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 BulletNumMax; // �ӵ�����

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TArray<TSubclassOf<ASProjectileBase>> BulletInGun; // ǹе���ִ��ӵ�

	int32 NowBulletType;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<TSubclassOf<ASProjectileBase>> BulletType;

	UFUNCTION(BlueprintCallable)
	void AddAll(); // װ��

	TSubclassOf<ASProjectileBase> BulletPop();

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
