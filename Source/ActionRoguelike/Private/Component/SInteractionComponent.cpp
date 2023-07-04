// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SInteractionComponent.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"
#include "SGmeplayInterface.h"
#include "DrawDebugHelpers.h"
#include "CollisionShape.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "UI/SWorldUserWidget.h"
#include "Blueprint/UserWidget.h"

// 交互Debug显示
static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.DebugDrawInteraction"), true, TEXT("Enable Debug Line For Interact Component."), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 500.0f;
	TraceRadius = 30.0f;
	CollisionChannel = ECC_WorldDynamic;
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 周期检测只能在客户端运行，不能在服务端运行
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}

}


// 周期检测
// 使用胶囊检测，从摄像头朝屏幕中心检测。把检测到的交互物体绑定提示UI。
void USInteractionComponent::FindBestInteractable()
{
	bool bDrawDebug = CVarDebugDrawInteraction.GetValueOnGameThread();

	// 设置碰撞检测物体
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	// 设置起点终点
	FVector Start, End;
	AActor* MyOwner = GetOwner();

	// 获取相机， 如果有相机组件就用相机组件计算起点终点，否则就用角色眼睛与转向
	UCameraComponent* CameraComp = Cast<UCameraComponent>(MyOwner->GetComponentByClass(UCameraComponent::StaticClass()));
	if (CameraComp)
	{
		// 设置起点，相机
		Start = CameraComp->GetComponentLocation();
		// 设置终点，相机到屏幕中间第一个触碰到的物体
		End = CameraComp->GetComponentLocation() + (CameraComp->GetComponentRotation().Vector() * TraceDistance);
	}
	else {
		FVector  EyeLocation; // 眼睛位置
		FRotator EyeRotation; // 眼睛朝向

		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		Start = EyeLocation;
		End = EyeLocation + (EyeRotation.Vector() * TraceDistance); // 终点
	}

	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);


	TArray<FHitResult> Hits; // 检测到的目标数组
	FCollisionShape Shape; // 检测物体形状
	Shape.SetSphere(TraceRadius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape); // 胶囊检测
	FColor HitColor = bBlockingHit ? FColor::Green : FColor::Red; // 根据是否检测到物体确定Debug颜色


	// 逐个调用接口
	AActor* FocusActorStaging = nullptr;
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		// 判断HitActor非空
		if (HitActor)
		{
			// 判断接口存在
			if (HitActor->Implements<USGmeplayInterface>())
			{
				FocusActorStaging = HitActor;

				// Debug并退出，因为只希望触发依次
				if (bDrawDebug)
				{
					DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, HitColor, false, 0.0f); // Debug球
				}

				break;
			}
		}

	}
	FocusActor = FocusActorStaging;

	// 存在交互物体
	if (FocusActor)
	{
		// 如果UI还没有被创建，就用默认提示UI类创建UI
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		// UI被创建后绑定在交互物体上
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttacheActor = FocusActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else // 不存在交互物体的时候删掉UI
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
}

// 1次交互
// 如果存在交互物体就调用接口
void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusActor);
}

// 服务器交互
void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor To Interact");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner()); // 传入Pawn
	ISGmeplayInterface::Execute_Interact(InFocus, MyPawn); // 调用接口
}