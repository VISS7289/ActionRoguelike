// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Projectile/SParryProjectile.h"
#include "Component/SActionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Game/SGameplayFunctionLibrary.h"
#include "Actions/SActionEffect.h"


// 组件碰撞事件处理函数
// 如果对方有反击标签，就被反击了，否则就正常攻击并附加火焰伤害
void ASParryProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator() && OtherActor->GetInstigator() != GetInstigator())
	{
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));

		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag)) // 被反击
		{
			MovementComp->Velocity = -MovementComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));

			return;
		}
		else if (ensure(HitEffect)) // 没被反击
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