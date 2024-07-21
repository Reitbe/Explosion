// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EPLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSION_API AEPLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEPLobbyPlayerController();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCreateSessionComplete(bool bWasSuccesssful);
	

// UI 
protected:
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<class UEPMainMenuWidget> MainMenuWidget;
};
