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

	MultiplayerSessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if(MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &UEPSessionBlockWidget::OnJoinSessionComplete);
	}

}

void UEPSessionBlockWidget::SetSessionFindResult(const FOnlineSessionSearchResult& SessionResult)
{
	SessionFindResult = SessionResult;

	int32 ConnectionCount = SessionResult.Session.NumOpenPublicConnections;
	int32 MaxConnectionCount = SessionResult.Session.SessionSettings.NumPublicConnections;
	
	//FString SessionName = SessionResult.Session.GetSessionIdStr();
	FString OwingUserName = SessionResult.Session.OwningUserName;
	FString PlayerCount = FString::Printf(TEXT("%02d/%02d"), ConnectionCount, MaxConnectionCount);
	FString Ping = FString::Printf(TEXT("%d"), SessionResult.PingInMs);

	SetSessionText(OwingUserName, PlayerCount, Ping);
}

void UEPSessionBlockWidget::SetSessionText(const FString& OwingUserName, const FString& AmountOfPlayers, const FString& Ping)
{
	if (ServerNameTextBlock)
	{
		ServerNameTextBlock->SetText(FText::FromString(OwingUserName));
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
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->JoinSession(SessionFindResult);
	}
}

void UEPSessionBlockWidget::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSessionPtr SessionInterface = *MultiplayerSessionSubsystem->GetSessionInterface();
	if (SessionInterface.IsValid())
	{
		FString Address;
		SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

		APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		if (PlayerController)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("조인세션 했고 트래블 간다!! : %s"), *Address));
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
	}
}
