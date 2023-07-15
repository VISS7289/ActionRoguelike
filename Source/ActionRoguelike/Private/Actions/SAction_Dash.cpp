// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_Dash.h"
#include "Kismet/KismetMathLibrary.h"


USAction_Dash::USAction_Dash()
{
    DashLength = 500.0f; 
    HasInit = false;
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
    // Do something with the timeline value
    NowPos = UKismetMathLibrary::VLerp(StartPos, EndPos, Value);

    if (!DashActor->SetActorLocation(NowPos, true))
    {
        CurveTimeline.Stop();
        StopAction(DashActor);
    }
}

// TimeLine播放结束
void USAction_Dash::TimelineCallbackFunction()
{
    StopAction(DashActor);
}

// 开始Action
void USAction_Dash::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);
    // 初始化
    if (!DashActor)
    {
        DashActor = InstigatorActor;
    }
    if (!HasInit)
    {
        SetupTimeline();
    }

    // 计算起点终点
    StartPos = InstigatorActor->GetActorLocation();

    FVector Fwd = InstigatorActor->GetActorForwardVector();
    EndPos = StartPos + Fwd * DashLength;

    // 播放TiemLine
    CurveTimeline.PlayFromStart();
}

void USAction_Dash::Tick(float DeltaTime)
{
    CurveTimeline.TickTimeline(DeltaTime);
}