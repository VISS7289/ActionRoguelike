// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

protected:

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh; // ǹе������

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<ASProjectileBase> DefaultBullet; // Ĭ���ӵ�

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int32 BulletNumMax; // �ӵ�����

	UPROPERTY(BlueprintReadWrite)
	TArray<TSubclassOf<AActor>> BulletInGun; // ǹе���ִ��ӵ�

	UFUNCTION(BlueprintCallable)
	void AddAll(); // װ��

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
