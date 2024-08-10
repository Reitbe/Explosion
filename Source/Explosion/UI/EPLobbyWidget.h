// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPLobbyWidget.generated.h"

UCLASS()
class EXPLOSION_API UEPLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SetPlayerCount(int32 PlayerInSession, int32 MaxPlayer);

protected:
	UFUNCTION()
	void OnReadyButtonClicked();

	UFUNCTION()
	void OnReturnToMainButtonClicked();

	UFUNCTION()
	void OnDestroySessionComplete(bool WasSuccessful);


protected:
	class UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ReadyButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ReturnToMainButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PlayerCounter;

private:
	FString LevelPath;
};
