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
    float rate = AccuStartAnim->GetPlayLength();
    CurveTimeline.SetPlayRate(1 / rate);
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

void USAction_Accumulate::Initialize_Implementation(USActionComponent* NewActionComp)
{
    Super::Initialize_Implementation(NewActionComp);

    AccuCharacter = Cast<ACharacter>(Owner);
    SpringArmComp = Cast<USpringArmComponent>(Owner->GetComponentByClass(USpringArmComponent::StaticClass()));
    ensure(SpringArmComp);
    StartPos = SpringArmComp->SocketOffset;
    EndPos = StartPos + FVector(1, 0, 0) * CameraAccumulate;

    SetupTimeline();
}


// Action开始
void USAction_Accumulate::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);

    // 播放
    if (ensure(AccuStartAnim) && ensure(CharacterAnimIns))
    {
        CharacterAnimIns->Montage_Play(AccuStartAnim);
        CharacterAnimIns->OnMontageBlendingOut.AddDynamic(this, &USAction_Accumulate::PlayAccuLoop);
    }
    CurveTimeline.PlayFromStart();
}

// Action结束
void USAction_Accumulate::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);
    // 停止TiemLine
    CurveTimeline.Stop();
    if (ensure(AttackAnim) && ensure(CharacterAnimIns))
    {
        CharacterAnimIns->OnMontageBlendingOut.RemoveDynamic(this, &USAction_Accumulate::PlayAccuLoop);
        CharacterAnimIns->Montage_Stop(0.0f, AccuStartAnim);
        CharacterAnimIns->Montage_Stop(0.0f, AccuLoopAnim);
        PlayFireAnim();
    }
    // 相机恢复
    SpringArmComp->SocketOffset = StartPos;
}

// 每Tick更新Timeline
void USAction_Accumulate::Tick(float DeltaTime)
{
    CurveTimeline.TickTimeline(DeltaTime);
}

void USAction_Accumulate::PlayAccuLoop(UAnimMontage* Montage, bool bInterrupted)
{
    if (ensure(AccuLoopAnim) && ensure(CharacterAnimIns))
    {
        UE_LOG(LogTemp, Log, TEXT("AccuLoop"));
        CharacterAnimIns->Montage_Play(AccuLoopAnim);
    }
}