// Fill out your copyright notice in the Description page of Project Settings.


#include "EPMultiplayerSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

UEPMultiplayerSessionSubsystem::UEPMultiplayerSessionSubsystem()
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
	, FindSessionCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionComplete))
	, JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete))
	, StartSessionCompletetDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
	, DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete))
	, SessionParticipantJoinedDelegate(FOnSessionParticipantJoinedDelegate::CreateUObject(this, &ThisClass::OnSessionParticipantJoined))
	, SessionParticipantLeftDelegate(FOnSessionParticipantLeftDelegate::CreateUObject(this, &ThisClass::OnSessionParticipantLeft))
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}
}

void UEPMultiplayerSessionSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	
	// 세션은 존재 확인
	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		bCreateSessionOnDestroy = true;
		LastNumPublicConnections = NumPublicConnections;
		LastMatchType = MatchType;

		DestroySession();
	}

	// Add CreateSessionCompleteDelegate to the session interface delegate list.
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	// Setting options for a new session
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : bIsLANMatchChecked; // 서브시스템이 연결되어있지 않다면 LAN 확정, 아니라면 bIsLANMatchChecked 값으로 설정
	LastSessionSettings->NumPublicConnections = NumPublicConnections; // 2 or 4 지정
	LastSessionSettings->bAllowJoinInProgress = true; 
	LastSessionSettings->bAllowJoinViaPresence = true; 
	LastSessionSettings->bShouldAdvertise = true; 
	LastSessionSettings->bUsesPresence = true; 
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->BuildUniqueId = 1;

	// Create Session
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings)) // 첫 인자는 FOnlineSessionSteam에서 HostingPlayerNum = 0으로 대체된다.
	{
		// Remove the delegate from the delegate list using the handle.
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		// Broadcast that the session creation has failed.
		MultiplayerOnCreateSessionComplete.Broadcast(false);
	}
}

void UEPMultiplayerSessionSubsystem::FindSession(int32 MaxSearchResults)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	// Add FindSessionCompleteDelegate to the session interface delegate list.
	FindSessionCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegate);

	// Setting session search options
	LastSessionSearch = MakeShareable(new FOnlineSessionSearch);
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : bIsLANMatchChecked;
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// Find sessions
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		// Clear the delegate from the delegate list
		SessionInterface->ClearOnCancelFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
		// Broadcast the result 
		MultiplayerOnFindSessionComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);

	}
}

void UEPMultiplayerSessionSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	// Add JoinSessionCompleteDelegate to the session interface delegate list.
	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		// Clear the delegate from the delegate list
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		// Broadcast the result 
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UEPMultiplayerSessionSubsystem::StartSession()
{
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnStartSessionComplete.Broadcast(false);
		return;
	}

	StartSessionCompleteDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompletetDelegate);
	if (!SessionInterface->StartSession(NAME_GameSession))
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
		MultiplayerOnStartSessionComplete.Broadcast(false);
	}
}

void UEPMultiplayerSessionSubsystem::DestroySession()
{
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}

	// Add DestroySessionCompleteDelegate to the session interface delegate list.
	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if (!SessionInterface->DestroySession(NAME_GameSession))
	{
		// Clear the delegate from the delegate list
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		// Broadcast the result 
		MultiplayerOnDestroySessionComplete.Broadcast(false);
	}
}

void UEPMultiplayerSessionSubsystem::OnSessionParticipantJoined(FName SessionName, const FUniqueNetId& PlayerId)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	
	SessionParticipantJoinedDelegateHandle = SessionInterface->AddOnSessionParticipantJoinedDelegate_Handle(SessionParticipantJoinedDelegate);
	MultiPlayerOnSessionParticipantJoined.Broadcast();
}

void UEPMultiplayerSessionSubsystem::OnSessionParticipantLeft(FName SessionName, const FUniqueNetId& PlayerId, EOnSessionParticipantLeftReason LeaveReason)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	SessionParticipantLeftDelegateHandle = SessionInterface->AddOnSessionParticipantLeftDelegate_Handle(SessionParticipantLeftDelegate);
	MultiPlayerOnSessionParticipantLeft.Broadcast();
}

const TCHAR* UEPMultiplayerSessionSubsystem::GetSessionState()
{
	const TCHAR* SessionState = ToString(SessionInterface->GetSessionState(NAME_GameSession));
	return SessionState;
}

FNamedOnlineSession* UEPMultiplayerSessionSubsystem::GetSession()
{
	if (!SessionInterface.IsValid())
	{
		return nullptr;
	}

	return SessionInterface->GetNamedSession(NAME_GameSession);
}

void UEPMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		// Claer the delegate from the delegate list using the handle
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("세션 생성 성공!! : %s"), *SessionName.ToString()));
	}

	// Broadcast a success message for the session creation.
	MultiplayerOnCreateSessionComplete.Broadcast(true);
}

void UEPMultiplayerSessionSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if (SessionInterface)
	{
		// Claer the delegate from the delegate list using the handle
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		// Session results was empty => false
		MultiplayerOnFindSessionComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	// Broadcast a success message for the session finding.
	MultiplayerOnFindSessionComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void UEPMultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface)
	{
		// Claer the delegate from the delegate list using the handle
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	if (!SessionInterface.IsValid())
	{
		return;
	}

	//if (Result == EOnJoinSessionCompleteResult::UnknownError)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("조인 세션 실패함."));
	//	return;
	//}
	//if (Result == EOnJoinSessionCompleteResult::Success)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("조인 세션 성공함."));

	//	FString Address;
	//	if (SessionInterface->GetResolvedConnectString(SessionName, Address))
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("세션이름 : %s"), *SessionName.ToString()));
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("주소 : %s"), *Address));

	//		APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	//		if (PlayerController)
	//		{
	//			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	//		}
	//	}
	//}


	// Broadcast a message for the session joining with the result.
	MultiplayerOnJoinSessionComplete.Broadcast(Result);
}

void UEPMultiplayerSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UEPMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}

	if (bWasSuccessful && bCreateSessionOnDestroy)
	{
		bCreateSessionOnDestroy = false;
		CreateSession(LastNumPublicConnections, LastMatchType);
	}

	MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}


