// Fill out your copyright notice in the Description page of Project Settings.


#include "EPLobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Explosion/Player/EPLobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Explosion/GameData/EPMultiplayerSessionSubsystem.h"

void UEPLobbyWidget::NativeConstruct()
{
	if (ReadyButton)
	{
		ReadyButton->OnClicked.AddDynamic(this, &UEPLobbyWidget::OnReadyButtonClicked);
	}

	if (ReturnToMainButton)
	{
		ReturnToMainButton->OnClicked.AddDynamic(this, &UEPLobbyWidget::OnReturnToMainButtonClicked);
	}

	MultiplayerSessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UEPLobbyWidget::OnDestroySessionComplete);
	}

	LevelPath = "/Game/Map/LobbyMap.LobbyMap";
}

void UEPLobbyWidget::SetPlayerCount(int32 PlayerInSession, int32 MaxPlayer)
{
	if (PlayerCounter)
	{
		PlayerCounter->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), PlayerInSession, MaxPlayer)));
	}
}

void UEPLobbyWidget::OnReadyButtonClicked()
{
	AEPLobbyPlayerController* LobbyPlayerController = Cast<AEPLobbyPlayerController>(GetGameInstance()->GetFirstLocalPlayerController());
	
	if (LobbyPlayerController)
	{
		LobbyPlayerController->SetReady();
		ReadyButton->SetIsEnabled(false);
	}
}

void UEPLobbyWidget::OnReturnToMainButtonClicked()
{
	ReadyButton->SetIsEnabled(true);
	
	// 세션에 참가했기에 로비에 있을 수 있다. 나가기 위해서 세션 파괴.
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->DestroySession();
	}
}

void UEPLobbyWidget::OnDestroySessionComplete(bool WasSuccessful)
{
	UGameplayStatics::OpenLevel(GetWorld(), *LevelPath);
	FString SessionState = MultiplayerSessionSubsystem->GetSessionState();
}
