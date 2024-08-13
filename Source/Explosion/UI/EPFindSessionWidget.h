// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EPFindSessionWidget.generated.h"

class UButton;
class UCheckBox;
class UScrollBox;
class UEPSessionBlockWidget;
class UEPMultiplayerSessionSubsystem;

/**
 * 로비에서 언리얼 온라인 서브시스템을 활용해 세션을 탐색하고, 그 결과를 출력하는 위젯.
 * 탐색 결과는 각 세션 결과 블록 위젯에 출력되며, 스크롤 박스에 추가된다.
 */

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
	UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> FindSessionButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> LANCheckBox;

	/* 세션 탐색 결과를 담을 블럭 위젯 */
	TSubclassOf<UEPSessionBlockWidget> SessionBlockWidgetClass;
	
	/* 세션 탐색 결과 블럭 위젯들을 담을 스크롤 박스 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> SessionListScrollBox;

	
private:
	/* 세션 탐색 조건으로 인터넷을 사용할 것인지 LAN을 사용할 것인지 나타낸다*/
	uint8 bUseLAN : 1;
	
	/* 세션 탐색 조건으로 사용되는 최대 탐색 수 */
	int32 MaxSearchResults;

	/* 세션 탐색 조건으로 사용되는 매치 공개 타입 */
	FString TypeOfMatch = FString(TEXT("FreeForAll"));
};
