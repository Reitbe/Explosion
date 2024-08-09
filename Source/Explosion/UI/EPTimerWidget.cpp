// Fill out your copyright notice in the Description page of Project Settings.


#include "EPTimerWidget.h"
#include "Components/TextBlock.h"
#include "Explosion/Player/EPPlayerController.h"
#include "Explosion/GameData/EPGameState.h"

void UEPTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	float ServerTime = 0.0f;
	float LimitTime = 0.0f;
	ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	if (LocalPlayer)
	{
		// 플레이어 컨트롤러에서 현재 서버 시간 가져오기
		EPPlayerController = Cast<AEPPlayerController>(LocalPlayer->PlayerController);
		if (EPPlayerController)
		{
			ServerTime = EPPlayerController->GetServerTime();
			EPPlayerController->OnSetupEndMatch.AddUObject(this, &UEPTimerWidget::StopTimeDisplay);
		}

		//게임 스테이트에서 시간 제한을 가져온다.
		EPGameState = GetWorld()->GetGameState<AEPGameState>();
		if (EPGameState)
		{
			LimitTime = EPGameState->GetMatchTimeLimit();
		}
	}
	InitialTime = LimitTime - ServerTime;

	DisplayTimerDelegate.BindUObject(this, &UEPTimerWidget::UpdateTimeDisplay);

}

void UEPTimerWidget::StartTimeDisplay()
{
	UpdateTimeDisplay(); // 최초 시간 출력

	GetWorld()->GetTimerManager().SetTimer(DisplayTimerHandle, DisplayTimerDelegate, 1.0f, true, -1.0f);
}

void UEPTimerWidget::UpdateTimeDisplay()
{
	InitialTime -= 1.0f;
	Minutes = int(InitialTime) / 60;
	Seconds = int(InitialTime) % 60;
	Minutes = FMath::Clamp(Minutes, 0.0f, Minutes);
	Seconds = FMath::Clamp(Seconds, 0.0f, Seconds);
	TimeDisplay->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds)));

	if (EPGameState->GetIsSetupEndMatch() || InitialTime <= 0)
	{
		StopTimeDisplay();
	}
}


void UEPTimerWidget::StopTimeDisplay()
{
	GetWorld()->GetTimerManager().ClearTimer(DisplayTimerHandle);
}

