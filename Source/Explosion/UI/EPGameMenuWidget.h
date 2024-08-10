// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPGameMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSION_API UEPGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	//UFUNCTION()
	//void OnSettingButtonClicked();

	UFUNCTION()
	void OnReturnToGameButtonClicked();

	UFUNCTION()
	void OnReturnToLobbyButtonClicked();

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ReturnToGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ReturnToLobbyButton;

	FString LevelPath;
};
