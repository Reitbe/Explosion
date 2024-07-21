// Fill out your copyright notice in the Description page of Project Settings.


#include "EPFindSessionWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ScrollBox.h"
#include "Explosion/UI/EPSessionBlockWidget.h"
#include "Explosion/GameData/EPMultiplayerSessionSubsystem.h"
#include "OnlineSessionSettings.h"

UEPFindSessionWidget::UEPFindSessionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> SessionBlockWidgetClassFinder
	(TEXT("/Game/UI/WBP_SessionBlockWidget.WBP_SessionBlockWidget_C"));
	if (SessionBlockWidgetClassFinder.Class)
	{
		SessionBlockWidgetClass = SessionBlockWidgetClassFinder.Class;
	}
}

void UEPFindSessionWidget::NativeConstruct()
{
	if (FindSessionButton)
	{
		FindSessionButton->OnClicked.AddDynamic(this, &UEPFindSessionWidget::OnFindSessionButtonClicked);
	}

	if(LANCheckBox)
	{
		LANCheckBox->OnCheckStateChanged.AddDynamic(this, &UEPFindSessionWidget::OnLANCheckBoxStateChanged);
	}

	UEPMultiplayerSessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if (SessionSubsystem)
	{
		SessionSubsystem->MultiplayerOnFindSessionComplete.AddUObject(this, &UEPFindSessionWidget::OnFindSessionComplete);
	}

	MaxSearchResults = 10;
}

void UEPFindSessionWidget::OnFindSessionButtonClicked()
{
	if (SessionListScrollBox)
	{
		SessionListScrollBox->ClearChildren();
	}

	if (FindSessionButton)
	{
		FindSessionButton->SetIsEnabled(false);
	}

	UEPMultiplayerSessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if (SessionSubsystem)
	{
		//SessionSubsystem->MultiplayerOnFindSessionComplete.AddUObject(this, &UEPFindSessionWidget::OnFindSessionComplete);
		SessionSubsystem->FindSession(MaxSearchResults, bUseLAN);
	}
}

void UEPFindSessionWidget::OnLANCheckBoxStateChanged(bool bIsChecked)
{
	bUseLAN = bIsChecked;
}

void UEPFindSessionWidget::OnFindSessionComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("세션 찾았다!!!"));
		for(const FOnlineSessionSearchResult& SessionResult : SessionResults)
		{
			UEPSessionBlockWidget* SessionBlockWidget = CreateWidget<UEPSessionBlockWidget>(GetWorld(), SessionBlockWidgetClass);
			if (SessionBlockWidget)
			{
				SessionBlockWidget->SetSessionFindResult(SessionResult);
				SessionListScrollBox->AddChild(SessionBlockWidget);
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("세션 찾기 실패했다!!!"));
	}

	if (FindSessionButton)
	{
		FindSessionButton->SetIsEnabled(true);
	}
}


