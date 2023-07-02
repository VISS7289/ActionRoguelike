// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "Component/SAttributeComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
	// ���������
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	// �������������
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	// Ϊ�¼�OnHealthChanged�󶨺���GetHealthChange
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::GetHealthChange);

}

// �˺�ʱ��˸
void ASTargetDummy::GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// ���˺�
	if (Delta < 0.0f)
	{
		// ������˸���Ѿ����ú��ˣ�ֻ��ҪTimeToHit����Ϸʱ����ͬ���ͻ���˸1s��
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
	
}
