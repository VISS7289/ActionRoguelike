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
    CurveTimeline.SetTimelineLength(1.0f);
    //CurveTimeline.SetPlayRate(5.0f);
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
    FVector RealPos = DashSCharacter->GetActorLocation();
    NowPos.Z = RealPos.Z;

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
    DashSCharacter->bSimGravityDisabled = true;
    StopAction(DashSCharacter);
}

void USAction_Dash::Initialize_Implementation(USActionComponent* NewActionComp)
{
    Super::Initialize_Implementation(NewActionComp);

    DashSCharacter = Cast<ASCharacter>(Owner);
    DashCamera = Cast<UCameraComponent>(DashSCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
    DashAnimIns = DashSCharacter->GetMesh()->GetAnimInstance();
    ensure(DashSCharacter);
    ensure(DashCamera);

    SetupTimeline();
}

// 开始Action
void USAction_Dash::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);
    
    // 计算起点终点
    StartPos = InstigatorActor->GetActorLocation();

    FVector Fwd = GetForward();
    EndPos = StartPos + Fwd * DashLength;

    // 播放
    if (ensure(SelectAnim) && ensure(DashAnimIns))
    {
        DashAnimIns->Montage_Play(SelectAnim);
        float rate = SelectAnim->GetPlayLength();
        CurveTimeline.SetPlayRate(1 / rate);
    }
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
    FVector ActorForward = DashCamera->GetForwardVector();
    if (Fwd.IsZero())
    {
        Fwd = DashSCharacter->GetPlayerMoveInput();
        if (Fwd.IsZero())
        {
            Fwd = ActorForward;
        }
        else
        {
            float YawAngle = GetYawByVector(FVector(1.0f, 0.0f, 0.0f), Fwd);
            FRotator Rot = FRotator(0.0f, YawAngle, 0.0f);
            Fwd = Rot.RotateVector(ActorForward);
        }
    }
    Fwd.Z = 0;
    SelectAnimByVector(ActorForward, Fwd);
    return Fwd.GetSafeNormal();
}

float USAction_Dash::GetYawByVector(FVector Fwd, FVector Act)
{
    float YawAngle = FMath::Acos(FVector::DotProduct(Fwd, Act) / (Act.Size() * Fwd.Size())) * 180.0f / PI;
    if (YawAngle != 0 && YawAngle != 180)
    {
        YawAngle *= FMath::Sign(FVector::CrossProduct(Fwd, Act).Z);
    }
    return YawAngle;
}

void USAction_Dash::SelectAnimByVector(FVector Fwd, FVector Act)
{
    float YawAngle = GetYawByVector(Fwd, Act);

    if (abs(YawAngle) < 45)
    {
        SelectAnim = FwdDashAnim;
        return;
    }
    if (abs(YawAngle) > 135)
    {
        SelectAnim = BackDashAnim;
        return;
    }
    if (abs(YawAngle - 90) <= 45)
    {
        SelectAnim = RightDashAnim;
        return;
    }
    if (ensure(abs(YawAngle + 90) <= 45))
    {
        SelectAnim = LeftDashAnim;
        return;
    }
}