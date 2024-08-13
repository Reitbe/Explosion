// Fill out your copyright notice in the Description page of Project Settings.


#include "EPLobbyCharacter.h"
#include "Explosion/Player/EPLobbyPlayerController.h"


AEPLobbyCharacter::AEPLobbyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEPLobbyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// 캐릭터 입력 비활성화
	APlayerController* LBPlayerController = Cast<AEPLobbyPlayerController>(GetController());
	DisableInput(LBPlayerController);
}


