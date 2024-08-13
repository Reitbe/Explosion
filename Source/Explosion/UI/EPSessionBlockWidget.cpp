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

void UEPSessionBlockWidget::ExtractSessionFindResult(const FOnlineSessionSearchResult& SessionResult)
{
	SessionFindResult = SessionResult;
	// 현재 세션에 접속 중인 플레이어 수
	int32 ConnectionCount = SessionResult.Session.NumOpenPublicConnections; 
	// 세션에 접속 가능한 최대 플레이어 수
	int32 MaxConnectionCount = SessionResult.Session.SessionSettings.NumPublicConnections;
	// 세션을 만든 유저 이름
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
		JoinSessionButton->SetIsEnabled(false);
		MultiplayerSessionSubsystem->JoinSession(SessionFindResult);
	}
}

void UEPSessionBlockWidget::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSessionPtr SessionInterface = *MultiplayerSessionSubsystem->GetSessionInterface();
	if (SessionInterface.IsValid())
	{
		if (Result == EOnJoinSessionCompleteResult::Success)
		{
			// 세션 이름을 사용하여 원하는 세션을 찾고, 그 주소를 가져온다.
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
		// 세션 침가 실패
		else 
		{
			JoinSessionButton->SetIsEnabled(true);
		}
	}
}
