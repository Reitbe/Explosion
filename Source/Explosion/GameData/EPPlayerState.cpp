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
	SetIsKillCountReplicated(true);

	AEPPlayerController* EPPlayerController = Cast<AEPPlayerController>(GetWorld()->GetFirstPlayerController());
	//EPPlayerController->ClientRPCUpdateScoreBoard();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("KillCount Updated"));
	EPPlayerController->UpdateScoreBoard();
}

void AEPPlayerState::OnRep_UpdateDeathCount()
{
	SetIsDeathCountReplicated(true);

	AEPPlayerController* EPPlayerController = Cast<AEPPlayerController>(GetWorld()->GetFirstPlayerController());
	//EPPlayerController->ClientRPCUpdateScoreBoard();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DeathCount Updated"));
	EPPlayerController->UpdateScoreBoard();
}

//FString AEPPlayerState::GetPlayerNameCustom() const
//{
//}
