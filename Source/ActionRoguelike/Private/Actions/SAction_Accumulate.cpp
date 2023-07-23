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

// ��ʼ��Timeline
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

// TimeLine���Ź����о۽����
void USAction_Accumulate::TimelineProgressFunction(float Value)
{
    CurrentAccumulateTime = Value;

    NowPos = UKismetMathLibrary::VLerp(StartPos, EndPos, Value);
    SpringArmComp->SocketOffset = NowPos;
}

void USAction_Accumulate::Initialize(USActionComponent* NewActionComp)
{
    Super::Initialize(NewActionComp);

    AccuCharacter = Cast<ACharacter>(Owner);
    AccuAnimIns = AccuCharacter->GetMesh()->GetAnimInstance();
    SpringArmComp = Cast<USpringArmComponent>(Owner->GetComponentByClass(USpringArmComponent::StaticClass()));
    ensure(SpringArmComp);
    ensure(AccuAnimIns);
    StartPos = SpringArmComp->SocketOffset;
    EndPos = StartPos + FVector(1, 0, 0) * CameraAccumulate;

    SetupTimeline();
}


// Action��ʼ
void USAction_Accumulate::StartAction_Implementation(AActor* InstigatorActor)
{
    Super::StartAction_Implementation(InstigatorActor);

    // ����
    if (ensure(AccuStartAnim) && ensure(AccuAnimIns))
    {
        AccuAnimIns->Montage_Play(AccuStartAnim);
        AccuAnimIns->OnMontageBlendingOut.AddDynamic(this, &USAction_Accumulate::PlayAccuLoop);
    }
    CurveTimeline.PlayFromStart();
}

// Action����
void USAction_Accumulate::StopAction_Implementation(AActor* InstigatorActor)
{
	Super::StopAction_Implementation(InstigatorActor);
    // ֹͣTiemLine
    CurveTimeline.Stop();
    if (ensure(AccuFireAnim) && ensure(AccuAnimIns))
    {
        AccuAnimIns->OnMontageBlendingOut.RemoveDynamic(this, &USAction_Accumulate::PlayAccuLoop);
        AccuAnimIns->Montage_Stop(0.0f, AccuStartAnim);
        AccuAnimIns->Montage_Stop(0.0f, AccuLoopAnim);
        AccuAnimIns->Montage_Play(AccuFireAnim);
        AccuAnimIns->OnPlayMontageNotifyBegin.AddDynamic(this, &USAction_Accumulate::FireNotify);
    }
    // ����ָ�
    SpringArmComp->SocketOffset = StartPos;
}
    


// ÿTick����Timeline
void USAction_Accumulate::Tick(float DeltaTime)
{
    CurveTimeline.TickTimeline(DeltaTime);
}

void USAction_Accumulate::PlayAccuLoop(UAnimMontage* Montage, bool bInterrupted)
{
    if (ensure(AccuLoopAnim) && ensure(AccuAnimIns))
    {
        UE_LOG(LogTemp, Log, TEXT("AccuLoop"));
        AccuAnimIns->Montage_Play(AccuLoopAnim);
    }
}

void USAction_Accumulate::FireNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    AccuAnimIns->OnPlayMontageNotifyBegin.RemoveDynamic(this, &USAction_Accumulate::FireNotify);

    // �����ӵ�
    if (ensure(AccuCharacter))
    {
        AttackDelay_Elapsed(AccuCharacter);
    }
}