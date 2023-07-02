// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "Component/SAttributeComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
	// 添加网格体
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	// 添加玩家属性组件
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	// 为事件OnHealthChanged绑定函数GetHealthChange
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::GetHealthChange);

}

// 伤害时闪烁
void ASTargetDummy::GetHealthChange(AActor* InstigatordActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// 被伤害
	if (Delta < 0.0f)
	{
		// 材质闪烁（已经设置好了，只需要TimeToHit与游戏时间相同，就会闪烁1s）
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
	
}
