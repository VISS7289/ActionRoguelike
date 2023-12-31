// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Queue.h"
#include "SWeaponComponent.generated.h"

class UStaticMeshComponent; // 静态网格体
class ASProjectileBase; // 子弹基类

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USWeaponComponent();

	// 返回Actor上的USWeaponComponent
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	static USWeaponComponent* GetWeaponComp(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	TSubclassOf<ASProjectileBase> Fire(); // 开火

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AddBullet(TSubclassOf<ASProjectileBase> ProjectileClass); // 装弹

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool CanFire();

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void BulletTypeRight(); // 向右旋转

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void BulletTypeLeft(); // 向左旋转

	TSubclassOf<ASProjectileBase> GetDefaultBullet();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh; // 枪械网格体

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<ASProjectileBase> DefaultBullet; // 默认子弹

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 BulletNumMax; // 子弹上限

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TArray<TSubclassOf<ASProjectileBase>> BulletInGun; // 枪械中现存子弹

	int32 NowBulletType;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<TSubclassOf<ASProjectileBase>> BulletType;

	UFUNCTION(BlueprintCallable)
	void AddAll(); // 装满

	TSubclassOf<ASProjectileBase> BulletPop();

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
