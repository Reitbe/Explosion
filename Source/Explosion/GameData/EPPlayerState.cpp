// Fill out your copyright notice in the Description page of Project Settings.


#include "EPPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Explosion/Player/EPPlayerController.h"

AEPPlayerState::AEPPlayerState()
{
	bUseCustomPlayerNames = true;
}

void AEPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEPPlayerState, KillCount);
	DOREPLIFETIME(AEPPlayerState, DeathCount);
}

void AEPPlayerState::OnRep_UpdateKillCount()
{
	AEPPlayerController* EPPlayerController = Cast<AEPPlayerController>(GetGameInstance()->GetFirstLocalPlayerController());
	EPPlayerController->UpdateScoreBoard();
}

void AEPPlayerState::OnRep_UpdateDeathCount()
{
	AEPPlayerController* EPPlayerController = Cast<AEPPlayerController>(GetGameInstance()->GetFirstLocalPlayerController());
	EPPlayerController->UpdateScoreBoard();
}


