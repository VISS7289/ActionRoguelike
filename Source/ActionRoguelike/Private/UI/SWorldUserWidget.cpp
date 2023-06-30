// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"


// 每帧修正敌人血条位置
void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 判断附着者是否存在或被销毁
	if (!IsValid(AttacheActor))
	{
		// 在窗口中移除UI
		RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("AttachedActor No Longger Valid, Removing Health Widget."))
		return;
	}

	// 将UI显示在屏幕上
	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttacheActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		// 对缩放修正
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;
		// 设置盒子组件位置
		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}