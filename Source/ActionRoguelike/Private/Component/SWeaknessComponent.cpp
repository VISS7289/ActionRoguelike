// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SWeaknessComponent.h"
#include "Item/SWeakness.h"
#include "GameFramework/Character.h"
#include "Component/SAttributeComponent.h"

// Sets default values for this component's properties
USWeaknessComponent::USWeaknessComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USWeaknessComponent::DestoryAllWeakness()
{
	for (int i = 0; i < AllWeakness.Num(); i++)
	{
		AllWeakness[i]->Destroy();
	}
}


// Called when the game starts
void USWeaknessComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACharacter>(GetOwner());
	OwnerAttributeComp = USAttributeComponent::GetAttributes(OwningCharacter);

	// 默认无位移，因为会绑定
	FTransform SpawnTM;
	// 设置生成物规则
	FActorSpawnParameters SpawnParames;
	SpawnParames.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParames.Instigator = OwningCharacter;

	for (int i = 0; i < Weaknesses.Num(); i++)
	{
		SpawnTM.SetScale3D(Weaknesses[i].WeaknessScale);
		AllWeakness.Add(GetWorld()->SpawnActor<ASWeakness>(Weaknesses[i].WeaknessActor, SpawnTM, SpawnParames));
		AllWeakness[i]->K2_AttachToComponent(OwningCharacter->GetMesh(), Weaknesses[i].WeaknessName, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		AllWeakness[i]->SetAttrComp(OwnerAttributeComp);
	}
	
}


// Called every frame
void USWeaknessComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

