// Fill out your copyright notice in the Description page of Project Settings.


#include "EPMainMenuWidget.h"
#include "Components/Button.h"
#include "Explosion/UI/EPCreateSessionWidget.h"
#include "Explosion/UI/EPFindSessionWidget.h"
#include "Explosion/GameData/EPMultiplayerSessionSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

void UEPMainMenuWidget::NativeConstruct()
{
	if(PlayGameButton)
	{
		PlayGameButton->OnClicked.AddDynamic(this, &UEPMainMenuWidget::OnPlayGameButtonClicked);
	}

	if(ExitGameButton)
	{
		ExitGameButton->OnClicked.AddDynamic(this, &UEPMainMenuWidget::OnExitGameButtonClicked);
	}

	if(CreateSessionWidget)
	{
		CreateSessionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(FindSessionWidget)
	{
		FindSessionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UEPMainMenuWidget::OnPlayGameButtonClicked()
{
	// 게임 플레이 버튼을 눌러야 세션의 생성과 탐색 위젯이 나타난다.
	if (CreateSessionWidget && FindSessionWidget)
	{
		if(CreateSessionWidget->GetVisibility() == ESlateVisibility::Collapsed && FindSessionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			CreateSessionWidget->SetVisibility(ESlateVisibility::Visible);
			FindSessionWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			CreateSessionWidget->SetVisibility(ESlateVisibility::Collapsed);
			FindSessionWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}


void UEPMainMenuWidget::OnExitGameButtonClicked()
{
	// 게임 프로그램 종료
	TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit; 
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, QuitPreference, true);
}
