// Fill out your copyright notice in the Description page of Project Settings.


#include "EPCreateSessionWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"
#include "Kismet/GameplayStatics.h"
#include "Explosion/GameData/EPMultiplayerSessionSubsystem.h"
#include "Explosion/Explosion.h"
// EP_LOG(LogExplosion, Log, TEXT("%s"), TEXT("전체 다 몽타주를 재생"));

void UEPCreateSessionWidget::NativeConstruct()
{
	//if (MaxPlayerEditableTextBox)
	//{
	//	MaxPlayerEditableTextBox->OnTextCommitted.AddDynamic(this, &UEPCreateSessionWidget::OnMaxPlayerTextCommitted);
	//}

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

	//LevelPath = "/Game/Map/BattleRoyalMap.BattleRoyalMap";
	//LevelPath = "/Game/Map/TravelTest.TravelTest?listen";
	LevelPath = "/Game/Map/LobbyMap.LobbyMap?listen";
	MaxPlayerNum = 2;
	SelectTwoPlayerButton->SetIsEnabled(false);
}

//void UEPCreateSessionWidget::OnMaxPlayerTextCommitted(const FText& text, ETextCommit::Type CommitMethod)
//{
//	MaxPlayerNum = FCString::Atoi(*text.ToString());
//}

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
	APlayerController* Player = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld());
	if (Player->IsLocalController())
	{
		if (bWasSuccessful)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("세션 생성 성공"));
			UGameplayStatics::OpenLevel(GetWorld(), *LevelPath);			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("세션 생성 실패"));
		}
	}

}
