// Fill out your copyright notice in the Description page of Project Settings.


#include "EPLobbyPlayerController.h"
#include "Explosion/UI/EPMainMenuWidget.h"
#include "Explosion/UI/EPLobbyWidget.h"
#include "Explosion/GameData/EPLobbyGameMode.h"
#include "Explosion/GameData/EPLobbyPlayerState.h"
#include "Explosion/GameData/EPMultiplayerSessionSubsystem.h"
#include "Explosion/Prop/EPLobbyStatueManager.h"
#include "Net/UnrealNetwork.h"

AEPLobbyPlayerController::AEPLobbyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UEPMainMenuWidget> MainMenuWidgetClassFinder
	(TEXT("/Game/UI/WBP_MainMenuWidget.WBP_MainMenuWidget_C"));
	if (MainMenuWidgetClassFinder.Class)
	{
		MainMenuWidgetClass = MainMenuWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UEPLobbyWidget> LobbyWidgetClassFinder
	(TEXT("/Game/UI/WBP_LobbyWidget.WBP_LobbyWidget_C"));
	if (LobbyWidgetClassFinder.Class)
	{
		LobbyWidgetClass = LobbyWidgetClassFinder.Class;
	}

	// 석상의 활성화를 관리할 석상 매니저
	StatueManager = CreateDefaultSubobject<UEPLobbyStatueManager>(TEXT("BombManager"));
}

void AEPLobbyPlayerController::BeginPlay()
{
	if (IsLocalController())
	{
		if (MainMenuWidgetClass)
		{
			MainMenuWidget = CreateWidget<UEPMainMenuWidget>(this, MainMenuWidgetClass);
			MainMenuWidget->AddToViewport();
			MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (LobbyWidgetClass)
		{
			LobbyWidget = CreateWidget<UEPLobbyWidget>(this, LobbyWidgetClass);
			LobbyWidget->AddToViewport();
			LobbyWidget->SetVisibility(ESlateVisibility::Collapsed);
		}

		MultiplayerSessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
		if (MultiplayerSessionSubsystem)
		{
			MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &AEPLobbyPlayerController::OnCreateSessionComplete);

			FString SessionState = MultiplayerSessionSubsystem->GetSessionState();

			// 세션이 존재하지 않으면 메인 메뉴에 있다고 판단하여 UI 관리
			if (SessionState == FString(TEXT("NoSession")))
			{
				MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
			}
			// 그렇지 않은 경우(InProgress, Pending)는 로비에 있다고 판단하여 UI관리
			else
			{
				LobbyWidget->SetVisibility(ESlateVisibility::Visible);
			}

			if (StatueManager)
			{
				StatueManager->GetAllLobbyStatue();
			}
		}
	}

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

void AEPLobbyPlayerController::OnCreateSessionComplete(bool bWasSuccesssful)
{
	if (bWasSuccesssful)
	{
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		LobbyWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AEPLobbyPlayerController::ServerRPC_SetReady_Implementation()
{
	// 플레이어 스테이트의 준비상태를 변경
	AEPLobbyPlayerState* LobbyPlayerState = GetPlayerState<AEPLobbyPlayerState>();
	if(LobbyPlayerState)
	{
		LobbyPlayerState->SetIsReady(true);
	}

	// 본 플레이어가 준비되었음을 게임모드에 전달.
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		AEPLobbyGameMode* LobbyGameMode = Cast<AEPLobbyGameMode>(GameMode);
		if (LobbyGameMode)
		{
			LobbyGameMode->UpdateReadyPlayerCount();
		}
	}
}

void AEPLobbyPlayerController::ClientRPC_UpdatePalyerCount_Implementation(int32 PlayerInSession, int32 MaxPlayer)
{
	// 서버로부터 현재 게임에 참가한 인원과 최대 참여 가능 인원을 전달받아 UI를 업데이트
	if (LobbyWidget)
	{
		LobbyWidget->SetPlayerCount(PlayerInSession, MaxPlayer);
	}
}

void AEPLobbyPlayerController::SetReady()
{
	// 클라이언트에서 준비가 되었음을 서버에게 알림
	if (IsLocalController())
	{
		ServerRPC_SetReady();
	}
}

void AEPLobbyPlayerController::ClientRPC_UpdateLobbyStatue_Implementation(bool IsOnMainMenu)
{
	if (IsLocalController() && StatueManager)
	{
		StatueManager->UpdateLobbyStatue(IsOnMainMenu);
	}
}
