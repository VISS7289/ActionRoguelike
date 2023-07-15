// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_Accumulate.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"


USAction_Accumulate::USAction_Accumulate()
{
	MaxAccumulateTime = 2.0f;
    CameraAccumulate = 100.0f;
}

void USAction_Accumulate::SetupTimeline()
{
    FOnTimelineFloat TimelineProgressFunction;
    TimelineProgressFunction.BindUFunction(this, FName("TimelineProgressFunction"));

    CurveTimeline.SetLooping(false);
    CurveTimeline.SetTimelineLength(MaxAccumulateTime);
    if (ensure(Curve))
    {
        CurveTimeline.AddInterpFloat(Curve, TimelineProgressFunction);
        HasInit = true;
    }
}

// TimeLine播放过程中
void USAction_Accumulate::TimelineProgressFunction(float Value)
{
    CurrentAccumulateTime = Value;

    NowPos = UKismetMathLibrary::VLerp(StartPos, EndPos, Value);
    //SpringArmComp->SetRelativeLocation(NowPos);
    SpringArmComp->SocketOffset = NowPos;
}

void USAction_Accumulate::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);

    if (!SpringArmComp)
    {
        SpringArmComp = Cast<USpringArmComponent>(InstigatorActor->GetComponentByClass(USpringArmComponent::StaticClass()));
        if (ensure(SpringArmComp))
        {
            
            StartPos = SpringArmComp->SocketOffset;
            EndPos = StartPos + FVector(1,0,0) * CameraAccumulate;
            //StartPos = SpringArmComp->GetRelativeLocation();
            //EndPos = StartPos + SpringArmComp->GetForwardVector() * CameraAccumulate;
        }
    }
    if (!HasInit)
    {
        SetupTimeline();
    }

    // 播放TiemLine
    CurveTimeline.PlayFromStart();
}

void USAction_Accumulate::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);

    CurveTimeline.Stop();

    ACharacter* Character = Cast<ACharacter>(InstigatorActor);
    if (Character)
    {
        AttackDelay_Elapsed(Character);
    }

    //SpringArmComp->SetRelativeLocation(StartPos);
    SpringArmComp->SocketOffset = StartPos;
}

void USAction_Accumulate::Tick(float DeltaTime)
{
    CurveTimeline.TickTimeline(DeltaTime);
}