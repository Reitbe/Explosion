// Fill out your copyright notice in the Description page of Project Settings.


#include "EPHUDWidget.h"
#include "Explosion/UI/EPHpBarWidget.h"

void UEPHUDWidget::NativeConstruct()
{
}

void UEPHUDWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	if (HpBarWidget)
	{
		HpBarWidget->UpdateHpBar(NewCurrentHp, NewMaxHp);
	}
}
