// Fill out your copyright notice in the Description page of Project Settings.


#include "EPGameEndWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Explosion/GameData/EPMultiplayerSessionSubsystem.h"

void UEPGameEndWidget::NativeConstruct()
{
	if (ReturnToLobbyButton)
	{
		ReturnToLobbyButton->OnClicked.AddDynamic(this, &UEPGameEndWidget::OnReturnToLobbyButtonClicked);
	}

	LevelPath = "/Game/Map/LobbyMap.LobbyMap";
}

void UEPGameEndWidget::OnReturnToLobbyButtonClicked()
{
	UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->DestroySession();
		UGameplayStatics::OpenLevel(GetWorld(), *LevelPath);
	}
}
