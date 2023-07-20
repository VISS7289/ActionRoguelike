// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_Flash.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"


USAction_Flash::USAction_Flash()
{
	FlashLength = 1000.0f;
}

// ��ʼAction
void USAction_Flash::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);
    // ��ʼ��
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

    // ��������յ�
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
    // ������ײ
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(InstigatorActor);
    // ���ü����
    FCollisionObjectQueryParams ObjParams;
    ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    ObjParams.AddObjectTypesToQuery(ECC_Pawn);
    // ���Ҽ��
    FHitResult Hit;
    if (GetWorld()->SweepSingleByObjectType(Hit, StartPos, EndPos, FQuat::Identity, ObjParams, CollShap, Params))
    {
        return Hit.ImpactPoint;
    }
    return EndPos;
}