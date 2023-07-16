// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Projectile/SParryProjectile.h"
#include "Component/SActionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Game/SGameplayFunctionLibrary.h"
#include "Actions/SActionEffect.h"


// �����ײ�¼�������
// ����Է��з�����ǩ���ͱ������ˣ�������������������ӻ����˺�
void ASParryProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator() && OtherActor->GetInstigator() != GetInstigator())
	{
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));

		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag)) // ������
		{
			MovementComp->Velocity = -MovementComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));

			return;
		}
		else if (ensure(HitEffect)) // û������
		{
			USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult);
			USGameplayFunctionLibrary::ApplyRage(GetInstigator(), OtherActor, Rage);
			Explode();

			if (ActionComp && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}

			return;
		}

	}
	
}