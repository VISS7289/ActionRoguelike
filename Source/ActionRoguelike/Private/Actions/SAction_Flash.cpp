// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_Flash.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"


USAction_Flash::USAction_Flash()
{
	FlashLength = 1000.0f;
}

// 开始Action
void USAction_Flash::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);
    // 初始化
    if (!FlashActor)
    {
        FlashActor = InstigatorActor;
        ACharacter* FlashCharacter = Cast<ACharacter>(FlashActor);
        if (ensure(FlashCharacter))
        {
            UCapsuleComponent* CapsuleComp = FlashCharacter->GetCapsuleComponent();
            CollShap = CapsuleComp->GetCollisionShape();

            CameraComp = Cast<UCameraComponent>(FlashCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
        }
    }

    // 计算起点终点
    FVector StartPos = InstigatorActor->GetActorLocation();

    //FVector Fwd = InstigatorActor->GetActorForwardVector();
    FVector Fwd = InstigatorActor->GetVelocity();
    if (Fwd.IsZero())
    {
        Fwd = CameraComp->GetForwardVector();
    }
    Fwd.Z = 0.0f;
    Fwd.Normalize();

    FVector EndPos = StartPos + Fwd * FlashLength;
    EndPos = CollisionCheck(InstigatorActor, StartPos, EndPos);

    FlashActor->SetActorLocation(EndPos);

    StopAction(InstigatorActor);
}

FVector USAction_Flash::CollisionCheck(AActor* InstigatorActor, FVector StartPos, FVector EndPos)
{
    // 设置碰撞
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(InstigatorActor);
    // 设置检测物
    FCollisionObjectQueryParams ObjParams;
    ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    ObjParams.AddObjectTypesToQuery(ECC_Pawn);
    // 胶囊检测
    FHitResult Hit;
    if (GetWorld()->SweepSingleByObjectType(Hit, StartPos, EndPos, FQuat::Identity, ObjParams, CollShap, Params))
    {
        return Hit.ImpactPoint;
    }
    return EndPos;
}