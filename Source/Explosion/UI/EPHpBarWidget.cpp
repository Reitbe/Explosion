// Fill out your copyright notice in the Description page of Project Settings.


#include "EPHpBarWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"


void UEPHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetHpBar();
	SetHpText();
}

void UEPHpBarWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	CurrentHp = NewCurrentHp;
	MaxHp = NewMaxHp;

	ensure(NewMaxHp > 0.f);
	SetHpBar();
	SetHpText();
}


void UEPHpBarWidget::SetHpBar()
{
	if (HpBar)
	{
		HpBar->SetPercent(CurrentHp / MaxHp);
	}
}

void UEPHpBarWidget::SetHpText()
{
	if (HpText)
	{
		HpText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHp, MaxHp)));
	}
}
