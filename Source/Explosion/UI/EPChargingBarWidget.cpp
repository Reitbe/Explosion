// Fill out your copyright notice in the Description page of Project Settings.


#include "EPChargingBarWidget.h"
#include "Components/ProgressBar.h"

void UEPChargingBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RepeatTimeInterval = 0.02f;
}

void UEPChargingBarWidget::StartChargingBar(float MaxChargingDuration)
{
	AlphaValue = 0.f;
	ElapsedTime = 0.f;
	GetWorld()->GetTimerManager().SetTimer(ChargingTimerHandle, FTimerDelegate::CreateLambda([&, MaxChargingDuration]()
		{
			// 차징을 시작한 이후 흐른 시간을 계산
			ElapsedTime += RepeatTimeInterval;

			// 전체 기간 대비 현재까지 흐른 시간의 비율을 얻는다
			AlphaValue = FMath::Clamp(ElapsedTime / MaxChargingDuration, 0.f, 1.f);
			if (ChargingBar)
			{
				ChargingBar->SetPercent(AlphaValue);
				ChargingBar->SetFillColorAndOpacity(FLinearColor(1.f, 1.f-AlphaValue, 0.f, 1.f));
			}

			if (MaxChargingDuration < ElapsedTime)
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

