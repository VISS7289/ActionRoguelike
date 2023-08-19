// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SActionWeapon.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Component/SWeaponComponent.h"
#include "Component/SActionComponent.h"
#include "Item/Projectile/SProjectileBase.h"
#include "Net/UnrealNetwork.h"



USActionWeapon::USActionWeapon()
{
	HandSocketName = "Muzzle_02"; // ���ַ����������
}

void USActionWeapon::Initialize_Implementation(USActionComponent* NewActionComp)
{
	Super::Initialize_Implementation(NewActionComp);

	FireCharacter = Cast<ACharacter>(Owner);
	CharacterAnimIns = FireCharacter->GetMesh()->GetAnimInstance();
	WeaponComp = USWeaponComponent::GetWeaponComp(FireCharacter);
	ensure(CharacterAnimIns);
	ensure(WeaponComp);
}

// �ж��Ƿ���Թ���
bool USActionWeapon::CanStart_Implementation(AActor* InstigatorActor)
{
	if (!Super::CanStart_Implementation(InstigatorActor))
	{
		GetOwningComponent()->StopActionByName(InstigatorActor, "Accumulate");
		return false;
	}

	if (ensure(WeaponComp))
	{
		if (!WeaponComp->CanFire())
		{
			GetOwningComponent()->StartActionByName(InstigatorActor, "Reload");
			return false;
		}
		return true;
	}

	return false;
}

// ʵʩ����
// ���ý��Ҽ��ȷ����Ļ�м�ɽӴ����ĵ�һ��Ŀ���λ�ã��ٴ����ַ���������
void USActionWeapon::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{

	TSubclassOf<ASProjectileBase> ProjectileBaseClass = WeaponComp->Fire(); // ������

	if (ProjectileBaseClass)
	{
		// ����λ��
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		// ���Ҽ��������
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		// ������ײ
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);
		// ���ü����
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		// ��������յ�
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);
		// ���Ҽ��
		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		// ���ݼ����������ת����
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		// ��ɫǰ��λ��
		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		// �������������
		FActorSpawnParameters SpawnParames;
		SpawnParames.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParames.Instigator = InstigatorCharacter;
		// ����������
		GetWorld()->SpawnActor<AActor>(ProjectileBaseClass, SpawnTM, SpawnParames);
	}
}

void USActionWeapon::PlayFireAnim()
{
	if (ensure(CharacterAnimIns))
	{
		CharacterAnimIns = FireCharacter->GetMesh()->GetAnimInstance();
		if (ensureAlways(CharacterAnimIns) && ensure(AttackAnim))
		{
			CharacterAnimIns->Montage_Play(AttackAnim);
			CharacterAnimIns->OnPlayMontageNotifyBegin.AddDynamic(this, &USActionWeapon::FireNotify);
			CharacterAnimIns->OnMontageBlendingOut.AddDynamic(this, &USActionWeapon::FireAnimEnd);
		}
	}
	
}

void USActionWeapon::FireNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	CharacterAnimIns->OnPlayMontageNotifyBegin.RemoveDynamic(this, &USActionWeapon::FireNotify);

	// �����ӵ�
	if (ensure(FireCharacter))
	{
		AttackDelay_Elapsed(FireCharacter);
	}
}

void USActionWeapon::FireAnimEnd(UAnimMontage* Montage, bool bInterrupted)
{
	CharacterAnimIns->OnMontageBlendingOut.RemoveDynamic(this, &USActionWeapon::FireAnimEnd);
}

void USActionWeapon::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);
}

void USActionWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionWeapon, WeaponComp);
	DOREPLIFETIME(USActionWeapon, FireCharacter);
	DOREPLIFETIME(USActionWeapon, CharacterAnimIns);
}