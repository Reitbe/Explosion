// Fill out your copyright notice in the Description page of Project Settings.


#include "EPGameMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Explosion/GameData/EPMultiplayerSessionSubsystem.h"
#include "Explosion/Player/EPPlayerController.h"

void UEPGameMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReturnToGameButton)
	{
		ReturnToGameButton->OnClicked.AddDynamic(this, &UEPGameMenuWidget::OnReturnToGameButtonClicked);
	}

	if (ReturnToLobbyButton)
	{
		ReturnToLobbyButton->OnClicked.AddDynamic(this, &UEPGameMenuWidget::OnReturnToLobbyButtonClicked);
	}

	LevelPath = "/Game/Map/LobbyMap.LobbyMap";
}


void UEPGameMenuWidget::OnReturnToGameButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("다시 게임으로 갑시다"));
	AEPPlayerController* EPPlayerController = Cast<AEPPlayerController>(GetOwningLocalPlayer()->GetPlayerController(GetWorld()));
	if (EPPlayerController)
	{
		EPPlayerController->HideGameMenu();
	}
}

void UEPGameMenuWidget::OnReturnToLobbyButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("로비로 갑시다"));
	UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->DestroySession();
		UGameplayStatics::OpenLevel(GetWorld(), *LevelPath);
	}
}

