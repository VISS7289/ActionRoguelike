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

// 初始化Timeline
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

// TimeLine播放过程中聚焦相机
void USAction_Accumulate::TimelineProgressFunction(float Value)
{
    CurrentAccumulateTime = Value;

    NowPos = UKismetMathLibrary::VLerp(StartPos, EndPos, Value);
    SpringArmComp->SocketOffset = NowPos;
}

// Action开始
void USAction_Accumulate::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);
    // 初始化相关数据
    if (!SpringArmComp)
    {
        SpringArmComp = Cast<USpringArmComponent>(InstigatorActor->GetComponentByClass(USpringArmComponent::StaticClass()));
        if (ensure(SpringArmComp))
        {
            
            StartPos = SpringArmComp->SocketOffset;
            EndPos = StartPos + FVector(1,0,0) * CameraAccumulate;
        }
    }
    if (!HasInit)
    {
        SetupTimeline();
    }

    // 播放TiemLine
    CurveTimeline.PlayFromStart();
}

// Action结束
void USAction_Accumulate::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);
    // 停止TiemLine
    CurveTimeline.Stop();
    // 发射子弹
    ACharacter* Character = Cast<ACharacter>(InstigatorActor);
    if (Character)
    {
        AttackDelay_Elapsed(Character);
    }
    // 相机恢复
    SpringArmComp->SocketOffset = StartPos;
}

// 每Tick更新Timeline
void USAction_Accumulate::Tick(float DeltaTime)
{
    CurveTimeline.TickTimeline(DeltaTime);
}