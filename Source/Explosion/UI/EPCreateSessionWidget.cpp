// Fill out your copyright notice in the Description page of Project Settings.


#include "EPCreateSessionWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"
#include "Kismet/GameplayStatics.h"
#include "Explosion/GameData/EPMultiplayerSessionSubsystem.h"


void UEPCreateSessionWidget::NativeConstruct()
{
	if (LANCheckBox)
	{
		LANCheckBox->OnCheckStateChanged.AddDynamic(this, &UEPCreateSessionWidget::OnLANCheckBoxStateChanged);
	}

	if (SelectTwoPlayerButton)
	{
		SelectTwoPlayerButton->OnClicked.AddDynamic(this, &UEPCreateSessionWidget::OnSelectTwoPlayerButtonClicked);
	}

	if (SelectFourPlayerButton)
	{
		SelectFourPlayerButton->OnClicked.AddDynamic(this, &UEPCreateSessionWidget::OnSelectFourPlayerButtonClicked);
	}

	if (CreateSessionButton)
	{
		CreateSessionButton->OnClicked.AddDynamic(this, &UEPCreateSessionWidget::OnCreateSessionButtonClicked);
	}

	MultiplayerSessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &UEPCreateSessionWidget::OnCreateSessionComplete);
	}

	LevelPath = "/Game/Map/LobbyMap.LobbyMap?listen";
	MaxPlayerNum = 2;
	SelectTwoPlayerButton->SetIsEnabled(false);
}

void UEPCreateSessionWidget::OnLANCheckBoxStateChanged(bool bIsChecked)
{
	bUseLAN = bIsChecked;
}

void UEPCreateSessionWidget::OnSelectTwoPlayerButtonClicked()
{
	SelectTwoPlayerButton->SetIsEnabled(false);
	SelectFourPlayerButton->SetIsEnabled(true);
	MaxPlayerNum = 2;
}

void UEPCreateSessionWidget::OnSelectFourPlayerButtonClicked()
{
	SelectTwoPlayerButton->SetIsEnabled(true);
	SelectFourPlayerButton->SetIsEnabled(false);
	MaxPlayerNum = 4;
}

void UEPCreateSessionWidget::OnCreateSessionButtonClicked()
{
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->SetLANMatch(bUseLAN);
		MultiplayerSessionSubsystem->CreateSession(MaxPlayerNum, TypeOfMatch);
	}
}

void UEPCreateSessionWidget::OnCreateSessionComplete(bool bWasSuccessful)
{
	APlayerController* Player = GetGameInstance()->GetFirstLocalPlayerController();
	if (Player->IsLocalController())
	{
		if (bWasSuccessful)
		{
			UGameplayStatics::OpenLevel(GetWorld(), *LevelPath);			
		}
	}
}

