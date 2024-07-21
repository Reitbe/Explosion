// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "EPSessionBlockWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSION_API UEPSessionBlockWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SetSessionFindResult(const FOnlineSessionSearchResult& SessionResult);
	void SetSessionText(const FString& ServerName, const FString& AmountOfPlayers, const FString& Ping);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> JoinSessionButton;

protected:
	UFUNCTION()
	void OnJoinSessionButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ServerNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> AmountOfPlayersTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> PingTextBlock;
	
	FOnlineSessionSearchResult SessionFindResult;


};
