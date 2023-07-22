// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_Dash.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/SCharacter.h"
#include "Camera/CameraComponent.h"


USAction_Dash::USAction_Dash()
{
    DashLength = 500.0f; // 冲刺距离
}

// 冲刺TimeLine设置
void USAction_Dash::SetupTimeline()
{
    FOnTimelineEvent TimelineCallback;
    TimelineCallback.BindUFunction(this, FName("TimelineCallbackFunction"));

    FOnTimelineFloat TimelineProgressFunction;
    TimelineProgressFunction.BindUFunction(this, FName("TimelineProgressFunction"));

    CurveTimeline.SetLooping(false);
    CurveTimeline.SetTimelineLength(0.2f);
    CurveTimeline.SetTimelineFinishedFunc(TimelineCallback);
    if (ensure(Curve))
    {
        CurveTimeline.AddInterpFloat(Curve, TimelineProgressFunction);
        HasInit = true;
    }
}

// TimeLine播放过程中
void USAction_Dash::TimelineProgressFunction(float Value)
{
    // 插值
    NowPos = UKismetMathLibrary::VLerp(StartPos, EndPos, Value);
    // 实时更新玩家位置
    if (!DashSCharacter->SetActorLocation(NowPos, true))
    {
        CurveTimeline.Stop();
        StopAction(DashSCharacter);
    }
}

// TimeLine播放结束
void USAction_Dash::TimelineCallbackFunction()
{
    StopAction(DashSCharacter);
}

void USAction_Dash::Initialize(USActionComponent* NewActionComp)
{
    Super::Initialize(NewActionComp);

    DashSCharacter = Cast<ASCharacter>(Owner);
    DashCamera = Cast<UCameraComponent>(DashSCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
    ensure(DashSCharacter);
    ensure(DashCamera);
}

// 开始Action
void USAction_Dash::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);
    // 初始化
    if (!HasInit)
    {
        SetupTimeline();
    }

    // 计算起点终点
    StartPos = InstigatorActor->GetActorLocation();

    FVector Fwd = GetForward();
    EndPos = StartPos + Fwd * DashLength;

    // 播放TiemLine
    CurveTimeline.PlayFromStart();
}

// 每Tick更新Timeline
void USAction_Dash::Tick(float DeltaTime)
{
    CurveTimeline.TickTimeline(DeltaTime);
}

FVector USAction_Dash::GetForward()
{
    FVector Fwd = DashSCharacter->GetVelocity();
    if (Fwd.IsZero())
    {
        Fwd = DashSCharacter->GetPlayerMoveInput();
        FVector ActorForward = DashCamera->GetForwardVector();
        if (Fwd.IsZero())
        {
            Fwd = ActorForward;
        }
        else
        {
            float YawAngle = FMath::Acos(FVector::DotProduct(FVector(1.0f, 0.0f, 0.0f), Fwd) / Fwd.Size()) * 180.0f / PI;
            if (YawAngle != 0 && YawAngle != 180)
            {
                YawAngle *= FMath::Sign(FVector::CrossProduct(FVector(1.0f, 0.0f, 0.0f), Fwd).Z);
            }
            FRotator Rot = FRotator(0.0f, YawAngle, 0.0f);
            Fwd = Rot.RotateVector(ActorForward);
        }
    }
    Fwd.Z = 0;
    return Fwd.GetSafeNormal();
}