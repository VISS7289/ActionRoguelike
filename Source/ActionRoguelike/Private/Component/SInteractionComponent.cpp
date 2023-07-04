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

// ����Debug��ʾ
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

	// ���ڼ��ֻ���ڿͻ������У������ڷ��������
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}

}


// ���ڼ��
// ʹ�ý��Ҽ�⣬������ͷ����Ļ���ļ�⡣�Ѽ�⵽�Ľ����������ʾUI��
void USInteractionComponent::FindBestInteractable()
{
	bool bDrawDebug = CVarDebugDrawInteraction.GetValueOnGameThread();

	// ������ײ�������
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	// ��������յ�
	FVector Start, End;
	AActor* MyOwner = GetOwner();

	// ��ȡ����� �����������������������������յ㣬������ý�ɫ�۾���ת��
	UCameraComponent* CameraComp = Cast<UCameraComponent>(MyOwner->GetComponentByClass(UCameraComponent::StaticClass()));
	if (CameraComp)
	{
		// ������㣬���
		Start = CameraComp->GetComponentLocation();
		// �����յ㣬�������Ļ�м��һ��������������
		End = CameraComp->GetComponentLocation() + (CameraComp->GetComponentRotation().Vector() * TraceDistance);
	}
	else {
		FVector  EyeLocation; // �۾�λ��
		FRotator EyeRotation; // �۾�����

		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		Start = EyeLocation;
		End = EyeLocation + (EyeRotation.Vector() * TraceDistance); // �յ�
	}

	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);


	TArray<FHitResult> Hits; // ��⵽��Ŀ������
	FCollisionShape Shape; // ���������״
	Shape.SetSphere(TraceRadius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape); // ���Ҽ��
	FColor HitColor = bBlockingHit ? FColor::Green : FColor::Red; // �����Ƿ��⵽����ȷ��Debug��ɫ


	// ������ýӿ�
	AActor* FocusActorStaging = nullptr;
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		// �ж�HitActor�ǿ�
		if (HitActor)
		{
			// �жϽӿڴ���
			if (HitActor->Implements<USGmeplayInterface>())
			{
				FocusActorStaging = HitActor;

				// Debug���˳�����Ϊֻϣ����������
				if (bDrawDebug)
				{
					DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, HitColor, false, 0.0f); // Debug��
				}

				break;
			}
		}

	}
	FocusActor = FocusActorStaging;

	// ���ڽ�������
	if (FocusActor)
	{
		// ���UI��û�б�����������Ĭ����ʾUI�ഴ��UI
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		// UI����������ڽ���������
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttacheActor = FocusActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else // �����ڽ��������ʱ��ɾ��UI
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
}

// 1�ν���
// ������ڽ�������͵��ýӿ�
void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusActor);
}

// ����������
void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor To Interact");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner()); // ����Pawn
	ISGmeplayInterface::Execute_Interact(InFocus, MyPawn); // ���ýӿ�
}