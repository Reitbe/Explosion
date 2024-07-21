// Fill out your copyright notice in the Description page of Project Settings.


#include "EPLobbyPlayerController.h"
#include "Explosion/UI/EPMainMenuWidget.h"
#include "Explosion/GameData/EPMultiplayerSessionSubsystem.h"

AEPLobbyPlayerController::AEPLobbyPlayerController()
{
	// 위젯
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetClassFinder
	(TEXT("/Game/UI/WBP_MainMenuWidget.WBP_MainMenuWidget_C"));
	if (MainMenuWidgetClassFinder.Class)
	{
		MainMenuWidgetClass = MainMenuWidgetClassFinder.Class;
	}
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
		}
	}

	UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &AEPLobbyPlayerController::OnCreateSessionComplete);
	}
}

void AEPLobbyPlayerController::OnCreateSessionComplete(bool bWasSuccesssful)
{
	if (bWasSuccesssful)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}
