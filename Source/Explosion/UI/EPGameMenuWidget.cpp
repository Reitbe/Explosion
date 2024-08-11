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
	AEPPlayerController* EPPlayerController = Cast<AEPPlayerController>(GetGameInstance()->GetFirstLocalPlayerController());
	if (EPPlayerController)
	{
		EPPlayerController->HideGameMenu();
	}
}

void UEPGameMenuWidget::OnReturnToLobbyButtonClicked()
{
	UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if (MultiplayerSessionSubsystem)
	{
		// 메인 메뉴로 돌아가는 경우 세션 파괴
		MultiplayerSessionSubsystem->DestroySession();
		UGameplayStatics::OpenLevel(GetWorld(), *LevelPath);
	}
}

