// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EPLobbyPlayerController.generated.h"

class UEPMultiplayerSessionSubsystem;
class UEPLobbyStatueManager;
class UEPMainMenuWidget;
class UEPLobbyWidget;

/**
 * 메인 메뉴와 로비에서 사용되는 플레이어 컨트롤러.
 * 플레이어의 준비 상태와 UI를 관리한다.
 */

UCLASS()
class EXPLOSION_API AEPLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEPLobbyPlayerController();

protected:
	virtual void BeginPlay() override;

// 로비 
public:
	/* 클라이언트가 준비 되었음을 서버에 전달 */
	void SetReady();

protected:
	/* 서버에서 플레이어 준비 상태 변경 및 게임모드에 상황 전달 */
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetReady();

public:
	/* 서버로부터 현재 게임에 참가한 인원과 최대 참여 가능 인원을 전달받아 UI를 업데이트  */
	UFUNCTION(Client, Reliable)
	void ClientRPC_UpdatePalyerCount(int32 PlayerInSession, int32 MaxPlayer);

	/* 클라이언트에 위치한 석상들의 활성화 업데이트 */
	UFUNCTION(Client, Reliable)
	void ClientRPC_UpdateLobbyStatue(bool IsOnMainMenu);


// 메인메뉴
protected:
	/* 레벨 이동 간 UI 비활성화 */
	UFUNCTION()
	void OnCreateSessionComplete(bool bWasSuccesssful);

protected:
	UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;
	
	/* 레벨에 존재하는 석상들의 활성화 여부를 관리하는 석상 매니저 */
	TObjectPtr<UEPLobbyStatueManager> StatueManager;

// UI 
protected:
	TSubclassOf<UEPMainMenuWidget> MainMenuWidgetClass;
	TObjectPtr<UEPMainMenuWidget> MainMenuWidget;

	TSubclassOf<UEPLobbyWidget> LobbyWidgetClass;
	TObjectPtr<UEPLobbyWidget> LobbyWidget;
};
