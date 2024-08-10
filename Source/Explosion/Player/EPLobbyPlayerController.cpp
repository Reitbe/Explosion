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
	// 위젯
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

	StatueManager = CreateDefaultSubobject<UEPLobbyStatueManager>(TEXT("BombManager"));
}

void AEPLobbyPlayerController::BeginPlay()
{
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

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
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SessionState: %s"), *SessionState));

			if (SessionState == FString(TEXT("NoSession")))
			{
				MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				LobbyWidget->SetVisibility(ESlateVisibility::Visible);
			}

			if (StatueManager)
			{
				StatueManager->GetAllLobbyStatue();
				//if (SessionState == FString(TEXT("NoSession")))
				//{
				//	StatueManager->UpdateLobbyStatue(true);
				//}
				//else
				//{
				//	StatueManager->UpdateLobbyStatue(false);
				//}
			}
		}

	ConsoleCommand(TEXT("stat fps"), true);
	}
}

void AEPLobbyPlayerController::OnCreateSessionComplete(bool bWasSuccesssful)
{
	if (bWasSuccesssful)
	{
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		LobbyWidget->SetVisibility(ESlateVisibility::Visible);
		// 로비 UI 띄우기

		//SetInputMode(FInputModeGameOnly());
		//bShowMouseCursor = false;
	}
}

void AEPLobbyPlayerController::ServerRPC_SetReady_Implementation()
{
	AEPLobbyPlayerState* LobbyPlayerState = GetPlayerState<AEPLobbyPlayerState>();
	if(LobbyPlayerState)
	{
		LobbyPlayerState->SetIsReady(true);
	}

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	AEPLobbyGameMode* LobbyGameMode = Cast<AEPLobbyGameMode>(GameMode);
	if (LobbyGameMode)
	{
		LobbyGameMode->UpdatePlayerCount();
	}
}

void AEPLobbyPlayerController::ClientRPC_UpdatePalyerCount_Implementation(int32 PlayerInSession, int32 MaxPlayer)
{
	if (LobbyWidget)
	{
		LobbyWidget->SetPlayerCount(PlayerInSession, MaxPlayer);
	}
}

void AEPLobbyPlayerController::SetReady()
{
	if (IsLocalController())
	{
		ServerRPC_SetReady();
	}
}

void AEPLobbyPlayerController::StartSession()
{
	MultiplayerSessionSubsystem->StartSession();
}

void AEPLobbyPlayerController::ClientRPC_UpdateLobbyStatue_Implementation(bool IsOnMainMenu)
{
	if (IsLocalController() && StatueManager)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("플컨 - UpdateStatue 호출")));
		StatueManager->UpdateLobbyStatue(IsOnMainMenu);
	}
}
