// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"


// ÿ֡��������Ѫ��λ��
void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// �жϸ������Ƿ���ڻ�����
	if (!IsValid(AttacheActor))
	{
		// �ڴ������Ƴ�UI
		RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("AttachedActor No Longger Valid, Removing Health Widget."))
		return;
	}

	// ��UI��ʾ����Ļ��
	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttacheActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		// ����������
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;
		// ���ú������λ��
		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}