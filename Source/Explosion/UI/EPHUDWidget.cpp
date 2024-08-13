// Fill out your copyright notice in the Description page of Project Settings.


#include "EPHUDWidget.h"
#include "Explosion/UI/EPTimerWidget.h"
#include "Explosion/UI/EPHpBarWidget.h"
#include "Components/ProgressBar.h"

void UEPHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEPHUDWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	if (HpBarWidget)
	{
		HpBarWidget->UpdateHpBar(NewCurrentHp, NewMaxHp);
	}
}

void UEPHUDWidget::StartTimer()
{
	if (TimerWidget)
	{
		TimerWidget->StartTimeDisplay();
	}
}
