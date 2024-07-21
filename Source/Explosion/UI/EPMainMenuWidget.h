// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPMainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSION_API UEPMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnPlayGameButtonClicked();

	UFUNCTION()
	void OnSettingsButtonClicked();

	UFUNCTION()
	void OnExitGameButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> PlayGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SettingsButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ExitGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEPFindSessionWidget> FindSessionWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEPCreateSessionWidget> CreateSessionWidget;

};
