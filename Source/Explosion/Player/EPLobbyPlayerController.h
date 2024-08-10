// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EPLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSION_API AEPLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEPLobbyPlayerController();
	void SetReady();

	void StartSession();

	UFUNCTION(Client, Reliable)
	void ClientRPC_UpdateLobbyStatue(bool IsOnMainMenu);

	UFUNCTION(Client, Reliable)
	void ClientRPC_UpdatePalyerCount(int32 PlayerInSession, int32 MaxPlayer);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCreateSessionComplete(bool bWasSuccesssful);

// 준비
protected:
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetReady();
	

protected:
	class UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;
	
	TObjectPtr<class UEPLobbyStatueManager> StatueManager;

// UI 
protected:
	TSubclassOf<class UEPMainMenuWidget> MainMenuWidgetClass;
	TObjectPtr<class UEPMainMenuWidget> MainMenuWidget;

	TSubclassOf<class UEPLobbyWidget> LobbyWidgetClass;
	TObjectPtr<class UEPLobbyWidget> LobbyWidget;
};
