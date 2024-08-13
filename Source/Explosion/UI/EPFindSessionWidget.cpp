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

	MultiplayerSessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnFindSessionComplete.AddUObject(this, &UEPFindSessionWidget::OnFindSessionComplete);
	}

	MaxSearchResults = 10;
}

void UEPFindSessionWidget::OnFindSessionButtonClicked()
{
	// 기존의 세션 블록들을 전부 제거
	if (SessionListScrollBox)
	{
		SessionListScrollBox->ClearChildren();
	}

	if (FindSessionButton)
	{
		FindSessionButton->SetIsEnabled(false);
	}

	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->SetLANMatch(bUseLAN);
		MultiplayerSessionSubsystem->FindSession(MaxSearchResults);
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
		for(const FOnlineSessionSearchResult& SessionResult : SessionResults)
		{
			// 각 세션 탐색 결과와 탐색 조건이 일치하는지 확인
			FString SettingValue;
			SessionResult.Session.SessionSettings.Get(FName("MatchType"), SettingValue);
			if (SettingValue == TypeOfMatch)
			{
				// 세션 정보를 담은 세션 블록 위젯 추가
				UEPSessionBlockWidget* SessionBlockWidget = CreateWidget<UEPSessionBlockWidget>(GetWorld(), SessionBlockWidgetClass);
				if (SessionBlockWidget)
				{
					SessionBlockWidget->ExtractSessionFindResult(SessionResult);
					SessionListScrollBox->AddChild(SessionBlockWidget);
				}
			}
		}
	}

	if (FindSessionButton)
	{
		FindSessionButton->SetIsEnabled(true);
	}
}


