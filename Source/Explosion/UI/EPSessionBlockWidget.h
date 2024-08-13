// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EPSessionBlockWidget.generated.h"

class UButton;
class UTextBlock;
class UEPMultiplayerSessionSubsystem;

/**
 * 메인메뉴에서 세션 검색을 진행했을 때, 검색된 세션의 정보를 보여주는 위젯.
 * 한 세션당 하나의 위젯이 배정되며 세션 생성 플레이어의 이름, 세션에 참여한 플레이어 수, 핑을 보여준다.
 */

UCLASS()
class EXPLOSION_API UEPSessionBlockWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/* 세션 검색 결과에서 위젯에 출력할 정보들을 추출 */
	void ExtractSessionFindResult(const FOnlineSessionSearchResult& SessionResult);

	/* ExtractSessionFindResult에서 추출한 정보들로 위젯을 업데이트 */
	void SetSessionText(const FString& OwingUserName, const FString& AmountOfPlayers, const FString& Ping);

protected:
	UFUNCTION()
	void OnJoinSessionButtonClicked();

	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result);

protected:
	/* 언리얼 온라인 서브시스템 */
	UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;
	FOnlineSessionSearchResult SessionFindResult;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ServerNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AmountOfPlayersTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PingTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> JoinSessionButton;

};
