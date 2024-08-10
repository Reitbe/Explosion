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

	//if(SettingsButton)
	//{
	//	SettingsButton->OnClicked.AddDynamic(this, &UEPMainMenuWidget::OnSettingsButtonClicked);
	//}

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

//void UEPMainMenuWidget::OnSettingsButtonClicked()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("세팅버튼 눌림"));
//
//	const IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
//	if (OnlineSub)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("서브시스템 : %s"), *OnlineSub->GetSubsystemName().ToString()));
//	}
//
//}

void UEPMainMenuWidget::OnExitGameButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("종료버튼 눌림"));

	TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit; 
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, QuitPreference, true);
}
