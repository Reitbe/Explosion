// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "EPFindSessionWidget.generated.h"


UCLASS()
class EXPLOSION_API UEPFindSessionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UEPFindSessionWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnFindSessionButtonClicked();

	UFUNCTION()
	void OnLANCheckBoxStateChanged(bool bIsChecked);

	void OnFindSessionComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> FindSessionButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCheckBox> LANCheckBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> SessionListScrollBox;

	TSubclassOf<class UEPSessionBlockWidget> SessionBlockWidgetClass;
	
	uint8 bUseLAN : 1;
	int32 MaxSearchResults;
};
