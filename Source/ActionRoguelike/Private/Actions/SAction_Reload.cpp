// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_Reload.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Component/SWeaponComponent.h"



bool USAction_Reload::CanStart_Implementation(AActor* InstigatorActor)
{
	if (!Super::CanStart_Implementation(InstigatorActor))
	{
		return false;
	}

	if (!WeaponComp)
	{
		WeaponComp = USWeaponComponent::GetWeaponComp(InstigatorActor);
		if (!WeaponComp)
		{
			return false;
		}
	}

	return true;
}


void USAction_Reload::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if (Character)
	{
		if (ensure(ReloadAnim))
		{
			Character->PlayAnimMontage(ReloadAnim);

			if (Character->HasAuthority())
			{
				FTimerHandle TimerHandle_AttackDelay;
				FTimerDelegate Delegate;
				Delegate.BindUFunction(this, "ReloadBullet_Elapsed", Character);

				float AttackDelay = 0.2f;

				GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackDelay, false);
			}

		}
	}
}


void USAction_Reload::ReloadBullet_Elapsed(ACharacter* InstigatorCharacter)
{
	WeaponComp->AddBullet(WeaponComp->GetDefaultBullet());
	StopAction(InstigatorCharacter);
}