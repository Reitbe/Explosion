// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EPMultiplayerSessionSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccesssful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bwasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMultiPlayerOnSessionParticipantJoined);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMultiPlayerOnSessionParticipantLeft);

UCLASS()
class EXPLOSION_API UEPMultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UEPMultiplayerSessionSubsystem();

	void CreateSession(int32 NumPublicConnections, FString MatchType);
	void FindSession(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	void StartSession();
	void DestroySession();

	void OnSessionParticipantJoined(FName SessionName, const FUniqueNetId& PlayerId);
	void OnSessionParticipantLeft(FName SessionName, const FUniqueNetId& PlayerId, EOnSessionParticipantLeftReason LeaveReason);

	IOnlineSessionPtr* GetSessionInterface() { return &SessionInterface; };
	const TCHAR* GetSessionState();
	class FNamedOnlineSession* GetSession();
	
	void SetLANMatch(bool bIsLAN) { bIsLANMatchChecked = bIsLAN; };

	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;
	FMultiplayerOnFindSessionComplete MultiplayerOnFindSessionComplete;
	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;
	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;
	FMultiPlayerOnSessionParticipantJoined MultiPlayerOnSessionParticipantJoined;
	FMultiPlayerOnSessionParticipantLeft MultiPlayerOnSessionParticipantLeft;


protected:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	bool bCreateSessionOnDestroy{ false };
	bool bIsLANMatchChecked{ false };
	int32 LastNumPublicConnections;
	FString LastMatchType;


	// OnlineSessionSubsystem에서 사용되는 대리자들이다.
	// 해당 결과를 받아 내부 대리자로 전달하여 사용할 것.
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate	FindSessionCompleteDelegate;
	FDelegateHandle	FindSessionCompleteDelegateHandle;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle	JoinSessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate	StartSessionCompletetDelegate;
	FDelegateHandle	StartSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle	DestroySessionCompleteDelegateHandle;

	FOnSessionParticipantJoinedDelegate SessionParticipantJoinedDelegate;
	FDelegateHandle SessionParticipantJoinedDelegateHandle;

	FOnSessionParticipantLeftDelegate SessionParticipantLeftDelegate;
	FDelegateHandle SessionParticipantLeftDelegateHandle;
};
