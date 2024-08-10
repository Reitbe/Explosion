// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPCreateSessionWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSION_API UEPCreateSessionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	//UFUNCTION()
	//void OnMaxPlayerTextCommitted(const FText& text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnLANCheckBoxStateChanged(bool bIsChecked);

	UFUNCTION()
	void OnSelectTwoPlayerButtonClicked();

	UFUNCTION()
	void OnSelectFourPlayerButtonClicked();

	UFUNCTION()
	void OnCreateSessionButtonClicked();

	UFUNCTION()
	void OnCreateSessionComplete(bool bWasSuccessful);

protected:
	class UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<class UEditableTextBox> MaxPlayerEditableTextBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SelectTwoPlayerButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SelectFourPlayerButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCheckBox> LANCheckBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> CreateSessionButton;
	
	int32 MaxPlayerNum;
	uint8 bUseLAN : 1;
	FString TypeOfMatch = FString(TEXT("FreeForAll"));
	FString LevelPath;
	FString LevelGameModeOption;
};
