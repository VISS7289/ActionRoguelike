// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"
#include "SGmeplayInterface.h"
#include "DrawDebugHelpers.h"
#include "CollisionShape.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

	// ...
}

// 1次交互
void USInteractionComponent::PrimaryInteract()
{
	// 设置碰撞检测物体
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
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
		End = CameraComp->GetComponentLocation() + (CameraComp->GetComponentRotation().Vector() * 500);
	}
	else {
		FVector  EyeLocation; // 眼睛位置
		FRotator EyeRotation; // 眼睛朝向
		
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		Start = EyeLocation;
		End = EyeLocation + (EyeRotation.Vector() * 500); // 终点
	}

	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	float Radius = 30.0f; // 胶囊检测半径

	TArray<FHitResult> Hits; // 检测到的目标数组
	FCollisionShape Shape; // 检测物体形状
	Shape.SetSphere(Radius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape); // 胶囊检测
	FColor HitColor = bBlockingHit ? FColor::Green : FColor::Red; // 根据是否检测到物体确定Debug颜色

	// 逐个调用接口
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		// 判断HitActor非空
		if (HitActor)
		{
			// 判断接口存在
			if (HitActor->Implements<USGmeplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner); // 传入Pawn
				ISGmeplayInterface::Execute_Interact(HitActor, MyPawn); // 调用接口
				// Debug并退出，因为只希望触发依次
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, HitColor, false, 2.0f); // Debug球
				break;
			}
		}
		
	}
	
	DrawDebugLine(GetWorld(), Start, End, HitColor, false, 2.0f, 0, 2.0f); // Debug线

}

