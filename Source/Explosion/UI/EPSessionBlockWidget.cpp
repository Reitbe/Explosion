// Fill out your copyright notice in the Description page of Project Settings.


#include "EPSessionBlockWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Explosion/GameData/EPMultiplayerSessionSubsystem.h"

void UEPSessionBlockWidget::NativeConstruct()
{
	if (JoinSessionButton)
	{
		JoinSessionButton->OnClicked.AddDynamic(this, &UEPSessionBlockWidget::OnJoinSessionButtonClicked);
	}
}

void UEPSessionBlockWidget::SetSessionFindResult(const FOnlineSessionSearchResult& SessionResult)
{
	SessionFindResult = SessionResult;

	int32 ConnectionCount = SessionResult.Session.NumOpenPublicConnections;
	int32 MaxConnectionCount = SessionResult.Session.SessionSettings.NumPublicConnections;
	
	FString SessionName = SessionResult.Session.GetSessionIdStr();
	FString PlayerCount = FString::Printf(TEXT("%02d/%02d"), ConnectionCount, MaxConnectionCount);
	FString Ping = FString::Printf(TEXT("%d"), SessionResult.PingInMs);

	SetSessionText(SessionName, PlayerCount, Ping);
}

void UEPSessionBlockWidget::SetSessionText(const FString& ServerName, const FString& AmountOfPlayers, const FString& Ping)
{
	if (ServerNameTextBlock)
	{
		ServerNameTextBlock->SetText(FText::FromString(ServerName));
	}

	if (AmountOfPlayersTextBlock)
	{
		AmountOfPlayersTextBlock->SetText(FText::FromString(AmountOfPlayers));
	}

	if (PingTextBlock)
	{
		PingTextBlock->SetText(FText::FromString(Ping+"ms"));
	}
}

void UEPSessionBlockWidget::OnJoinSessionButtonClicked()
{
	UEPMultiplayerSessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if (SessionSubsystem)
	{
		SessionSubsystem->JoinSession(SessionFindResult);
	}
}
