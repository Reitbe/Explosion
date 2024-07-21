// Fill out your copyright notice in the Description page of Project Settings.


#include "EPChargingBarWidget.h"
#include "Components/ProgressBar.h"

void UEPChargingBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RepeatTimeInterval = 0.02f;
}

void UEPChargingBarWidget::StartChargingBar(float ChargingDuration)
{
	AlphaValue = 0.f;
	ElapsedTime = 0.f;
	GetWorld()->GetTimerManager().SetTimer(ChargingTimerHandle, FTimerDelegate::CreateLambda([&, ChargingDuration]()
		{
			ElapsedTime += RepeatTimeInterval;
			AlphaValue = FMath::Clamp(ElapsedTime / ChargingDuration, 0.f, 1.f);
			if (ChargingBar)
			{
				ChargingBar->SetPercent(AlphaValue);
				ChargingBar->SetFillColorAndOpacity(FLinearColor(1.f, 1.f-AlphaValue, 0.f, 1.f));
			}

			if (ChargingDuration < ElapsedTime)
			{
				EndChargingBar();
			}	
		}
	), RepeatTimeInterval, true);
}

void UEPChargingBarWidget::EndChargingBar()
{
	GetWorld()->GetTimerManager().ClearTimer(ChargingTimerHandle);
}

