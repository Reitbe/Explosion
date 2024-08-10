// Fill out your copyright notice in the Description page of Project Settings.


#include "EPLobbyCharacter.h"
#include "Explosion/Player/EPLobbyPlayerController.h"


// Sets default values
AEPLobbyCharacter::AEPLobbyCharacter()
{
}

// Called when the game starts or when spawned
void AEPLobbyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// 캐릭터 입력 비활성화
	APlayerController* LBPlayerController = Cast<AEPLobbyPlayerController>(GetController());
	DisableInput(LBPlayerController);
}

// Called every frame
void AEPLobbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

