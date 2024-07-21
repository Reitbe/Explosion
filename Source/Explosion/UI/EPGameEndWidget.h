// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPGameEndWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSION_API UEPGameEndWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReturnToLobbyButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ReturnToLobbyButton;

	FString LevelPath;


};
