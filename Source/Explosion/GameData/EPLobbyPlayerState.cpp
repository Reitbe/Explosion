// Fill out your copyright notice in the Description page of Project Settings.


#include "EPLobbyPlayerState.h"
#include "Net/UnrealNetwork.h"

AEPLobbyPlayerState::AEPLobbyPlayerState()
{
	bIsReady = false;
}

void AEPLobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEPLobbyPlayerState, bIsReady);
}
