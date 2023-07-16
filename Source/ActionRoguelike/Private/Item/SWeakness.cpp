// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SWeakness.h"
#include "Components/StaticMeshComponent.h"
#include "Component/SAttributeComponent.h"

// Sets default values
ASWeakness::ASWeakness()
{
	// 添加网格体
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetCollisionProfileName("Weakness");
	// 添加玩家属性组件
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASWeakness::GetHealthChange);

	DamageMultiply = 2.0f;
}

void ASWeakness::SetAttrComp(USAttributeComponent* AttrComp)
{
	OwnerAttributeComp = AttrComp;
}


// 弱点伤害处理
// 角色受到伤害后，如果该部位为弱点，追加伤害
void ASWeakness::GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta > 0)
	{
		return;
	}

	// 保证弱点本身属性的无敌
	AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax());

	if (InstigatordActor == GetInstigator())
	{
		return;
	}

	OwnerAttributeComp->ApplyHealthChange(InstigatordActor, Delta * DamageMultiply);
	
}