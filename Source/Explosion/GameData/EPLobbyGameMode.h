// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EPLobbyGameMode.generated.h"


UCLASS()
class EXPLOSION_API AEPLobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AEPLobbyGameMode();
	virtual void StartPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual bool CanServerTravel(const FString& URL, bool bAbsolute) override;

	void UpdatePlayerCount();

protected:
	UFUNCTION()
	void OnCreateSessionComplete(bool bWasSuccessful);

	UFUNCTION()
	void OnSessionParticipantJoined();

	UFUNCTION()
	void OnSessionParticipantLeft();

	void UpdateLobbyStatue();
	void StartMainGame();

protected:
	class UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> LobbyStatueWithPedestalsArray;

	TSubclassOf<class AEPLobbyStatue> LobbyStatueWithPedestalsClass;

private:
	int32 ReadyPlayerCount;
	int32 MaxPlayers;

	uint8 bIsFirstLoginCompleted : 1;

	FString MainGameMapName;
};
