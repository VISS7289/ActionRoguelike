// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"
#include "SGmeplayInterface.h"
#include "DrawDebugHelpers.h"
#include "CollisionShape.h"


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

// 1�ν���
void USInteractionComponent::PrimaryInteract()
{
	// ������ײ�������
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	// ��������յ�
	FVector  EyeLocation; // ���
	FRotator EyeRotation; // �۾�����
	AActor* MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000); // �յ�

	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	float Radius = 30.0f; // ���Ҽ��뾶

	TArray<FHitResult> Hits; // ��⵽��Ŀ������
	FCollisionShape Shape; // ���������״
	Shape.SetSphere(Radius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape); // ���Ҽ��
	FColor HitColor = bBlockingHit ? FColor::Green : FColor::Red; // �����Ƿ��⵽����ȷ��Debug��ɫ

	// ������ýӿ�
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		// �ж�HitActor�ǿ�
		if (HitActor)
		{
			// �жϽӿڴ���
			if (HitActor->Implements<USGmeplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner); // ����Pawn
				ISGmeplayInterface::Execute_Interact(HitActor, MyPawn); // ���ýӿ�
				// Debug���˳�����Ϊֻϣ����������
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, HitColor, false, 2.0f); // Debug��
				break;
			}
		}
		
	}
	
	DrawDebugLine(GetWorld(), EyeLocation, End, HitColor, false, 2.0f, 0, 2.0f); // Debug��

}

