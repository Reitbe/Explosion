// Fill out your copyright notice in the Description page of Project Settings.


#include "EPNameTagWidget.h"
#include "Components/TextBlock.h"

void UEPNameTagWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEPNameTagWidget::SetNameTag(FString Name)
{
	TxtNameTag->SetText(FText::FromString(Name));
}

