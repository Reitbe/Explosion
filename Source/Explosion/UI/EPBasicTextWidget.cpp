// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBasicTextWidget.h"
#include "Components/TextBlock.h"

void UEPBasicTextWidget::NativeConstruct()
{
}

void UEPBasicTextWidget::SetText(FText text)
{
	if (BasicTextBlock)
	{
		BasicTextBlock->SetText(text);
	}
}

void UEPBasicTextWidget::SetTextColorAndOpacity(FLinearColor NewColorAndOpacity)
{
	if (BasicTextBlock)
	{
		BasicTextBlock->SetColorAndOpacity(NewColorAndOpacity);
	}
}
