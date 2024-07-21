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
	if (MaxPlayerEditableTextBox)
	{
		MaxPlayerEditableTextBox->OnTextCommitted.AddDynamic(this, &UEPCreateSessionWidget::OnMaxPlayerTextCommitted);
	}

	if (LANCheckBox)
	{
		LANCheckBox->OnCheckStateChanged.AddDynamic(this, &UEPCreateSessionWidget::OnLANCheckBoxStateChanged);
	}

	if (CreateSessionButton)
	{
		CreateSessionButton->OnClicked.AddDynamic(this, &UEPCreateSessionWidget::OnCreateSessionButtonClicked);
	}

	UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &UEPCreateSessionWidget::OnCreateSessionComplete);
	}

	//LevelPath = "/Game/Map/BattleRoyalMap.BattleRoyalMap";
	LevelPath = "/Game/Map/TravelTest.TravelTest";
}

void UEPCreateSessionWidget::OnMaxPlayerTextCommitted(const FText& text, ETextCommit::Type CommitMethod)
{
	MaxPlayerNum = FCString::Atoi(*text.ToString());
}

void UEPCreateSessionWidget::OnLANCheckBoxStateChanged(bool bIsChecked)
{
	bUseLAN = bIsChecked;
}

void UEPCreateSessionWidget::OnCreateSessionButtonClicked()
{
	APlayerController* Player = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld());
	UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if (MultiplayerSessionSubsystem)
	{
		//MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &UEPCreateSessionWidget::OnCreateSessionComplete);
		MultiplayerSessionSubsystem->CreateSession(MaxPlayerNum, bUseLAN ? "LAN" : "Internet");
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
