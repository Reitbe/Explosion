// Fill out your copyright notice in the Description page of Project Settings.


#include "EPPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Explosion/UI/EPGameMenuWidget.h"
#include "Explosion/UI/EPScoreBoardWidget.h"
#include "Explosion/UI/EPGameEndWidget.h"
#include "Explosion/GameData/EPGameState.h"
#include "Explosion/GameData/EPPlayerState.h"
#include "Components/Button.h"



AEPPlayerController::AEPPlayerController()
{

// 입력
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder
	(TEXT("/Game/Input/IMC_PlayerController.IMC_PlayerController"));
	if (InputMappingContextFinder.Succeeded())
	{
		InputMappingContext = InputMappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ScoreBoardInputActionFinder
	(TEXT("/Game/Input/IA_ShowScoreBoard.IA_ShowScoreBoard"));
	if (ScoreBoardInputActionFinder.Succeeded())
	{
		ShowingScoreBoard = ScoreBoardInputActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> GameMenuInputActionFinder
	(TEXT("/Game/Input/IA_ShowGameMenu.IA_ShowGameMenu"));
	if (GameMenuInputActionFinder.Succeeded())
	{
		ShowingGameMenu = GameMenuInputActionFinder.Object;
	}
	
// 위젯
	static ConstructorHelpers::FClassFinder<UUserWidget> MatchEndWidgetClassFinder
	(TEXT("/Game/UI/WBP_MatchEndWidget.WBP_MatchEndWidget_C"));
	if (MatchEndWidgetClassFinder.Class)
	{
		MatchEndWidgetClass = MatchEndWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UEPGameMenuWidget> GameMenuWidgetClassFinder
	(TEXT("/Game/UI/WBP_GameMenuWidget.WBP_GameMenuWidget_C"));
	if (GameMenuWidgetClassFinder.Class)
	{
		GameMenuWidgetClass = GameMenuWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UEPScoreBoardWidget> ScoreBoardWidgetClassFinder
	(TEXT("/Game/UI/WBP_ScoreBoardWidget.WBP_ScoreBoardWidget_C"));
	if (ScoreBoardWidgetClassFinder.Class)
	{
		ScoreBoardWidgetClass = ScoreBoardWidgetClassFinder.Class;
	}

	ServerTime = 0.0f;
}

void AEPPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AEPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr) {
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	if (IsLocalController())
	{
		if (ScoreBoardWidgetClass)
		{
			ScoreBoardWidget = CreateWidget<UEPScoreBoardWidget>(this, ScoreBoardWidgetClass);
			ScoreBoardWidget->AddToViewport();
			ScoreBoardWidget->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (GameMenuWidgetClass)
		{
			GameMenuWidget = CreateWidget<UEPGameMenuWidget>(this, GameMenuWidgetClass);
			GameMenuWidget->AddToViewport();
			GameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (MatchEndWidgetClass)
		{
			MatchEndWidget = CreateWidget<UEPGameEndWidget>(this, MatchEndWidgetClass);
			MatchEndWidget->AddToViewport();
			MatchEndWidget->SetVisibility(ESlateVisibility::Collapsed);
		} 

		OnSetupEndMatch.AddUObject(this, &AEPPlayerController::SetupEndMatch);
	}
}

void AEPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent)) 
	{
		Input->BindAction(ShowingScoreBoard, ETriggerEvent::Started, this, &AEPPlayerController::ShowScoreBoard);
		Input->BindAction(ShowingScoreBoard, ETriggerEvent::Completed, this, &AEPPlayerController::HideScoreBoard);
		Input->BindAction(ShowingGameMenu, ETriggerEvent::Started, this, &AEPPlayerController::ShowGameMenu);

	}
}

void AEPPlayerController::ClientRPCUpdateScoreBoard_Implementation()
{
	if (IsLocalController() && ScoreBoardWidget)
	{
		ScoreBoardWidget->UpdateScoreBoard();
	}
}

void AEPPlayerController::ClientRPCSetupEndMatch_Implementation()
{
	OnSetupEndMatch.Broadcast();
}

void AEPPlayerController::SetupEndMatch()
{

	if (IsLocalController())
	{
		MatchEndWidget->SetVisibility(ESlateVisibility::Visible);
		ShowScoreBoard();
	}
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
	//DisableInput(this);
	SetIgnoreMoveInput(true);
	GetPawn()->DisableInput(this);
}

void AEPPlayerController::ClientRPCEndMatch_Implementation()
{
	if (IsLocalController())
	{
		MatchEndWidget->OnReturnToLobbyButtonClicked();
	}
}

void AEPPlayerController::ShowScoreBoard()
{
	if (ScoreBoardWidget)
	{
		ScoreBoardWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AEPPlayerController::HideScoreBoard()
{
	if (ScoreBoardWidget)
	{
		ScoreBoardWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AEPPlayerController::ShowGameMenu()
{	
	if (GameMenuWidget)
	{
		GameMenuWidget->SetVisibility(ESlateVisibility::Visible);
		GetPawn()->DisableInput(this);
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}

void AEPPlayerController::HideGameMenu()
{
	if (GameMenuWidget)
	{
		GameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		GetPawn()->EnableInput(this);
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void AEPPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (IsLocalController())
	{
		ServerRequestServerTime(this, GetWorld()->GetTimeSeconds());
	}
}

void AEPPlayerController::ClientReportServerTime_Implementation(float requestWorldTime, float serverTime)
{
	float roundTripTime = GetWorld()->GetTimeSeconds() - requestWorldTime;
	float adjustedTime = serverTime + (roundTripTime * 0.5f);
	ServerTime = adjustedTime;
}

void AEPPlayerController::ServerRequestServerTime_Implementation(APlayerController* requestor, float requestWorldTime)
{
	float serverTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	ClientReportServerTime(requestWorldTime, serverTime);
}

bool AEPPlayerController::ServerRequestServerTime_Validate(APlayerController* requestor, float requestWorldTime)
{
	return true;
}




