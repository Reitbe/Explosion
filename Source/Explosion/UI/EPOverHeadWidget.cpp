// Fill out your copyright notice in the Description page of Project Settings.


#include "EPOverHeadWidget.h"
#include "Explosion/UI/EPNameTagWidget.h"
#include "Explosion/UI/EPHpBarWidget.h"

void UEPOverHeadWidget::NativeConstruct()
{
}

void UEPOverHeadWidget::SetHpBarVisible(bool bVisible)
{
	if (HpBarWidget)
	{
		HpBarWidget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UEPOverHeadWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	if (HpBarWidget)
	{
		HpBarWidget->UpdateHpBar(NewCurrentHp, NewMaxHp);
	}
}

void UEPOverHeadWidget::UpdateNameTag(FString Name)
{
	if (NameTagWidget)
	{
		NameTagWidget->SetNameTag(Name);
	}
}