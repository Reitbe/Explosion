// Fill out your copyright notice in the Description page of Project Settings.


#include "EPLobbyGameMode.h"
#include "Explosion/Player/EPLobbyPlayerController.h"
#include "Explosion/Character/EPLobbyCharacter.h"

AEPLobbyGameMode::AEPLobbyGameMode()
{
	PlayerControllerClass = AEPLobbyPlayerController::StaticClass();
	DefaultPawnClass = AEPLobbyCharacter::StaticClass();
}
