// Fill out your copyright notice in the Description page of Project Settings.


#include "EPNameTagWidget.h"

void UEPNameTagWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (TxtNameTag)
	{
		TxtNameTag->SetText(FText::FromString("TestName"));
	}
}
