// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EPPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UUserWidget;
class UEPGameEndWidget;
class UEPScoreBoardWidget;
class UEPGameMenuWidget;
class UEPBasicTextWidget;

DECLARE_MULTICAST_DELEGATE(FOnSetupEndMatch)
DECLARE_MULTICAST_DELEGATE(FOnStartMainGame)

/**
 * 본 게임에서 사용되는 플레이어 컨트롤러. 
 * 게임 진행에 따른 UI와 입력 관리를 담당하며, 이를 위해 서버와 시간 동기화를 진행한다.
 */

UCLASS()
class EXPLOSION_API AEPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEPPlayerController();
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

	/* 플레이어 컨트롤러가 네트워크에 연결되었을 때 호출. 서버에게 시간을 요청한다. */
	virtual void ReceivedPlayer() override;

protected:
	/* 게임에 참가한 모든 플레이어가 준비되었을 때, 입력과 UI를 활성화한다 */
	UFUNCTION()
	void StartMainGame();

// 매치 시작 및 종료
public:
	/* 매치가 종료되었을 때 실행할 동작들을 등록하는 대리자*/
	FOnSetupEndMatch OnSetupEndMatch;

	/* 매치가 시작되었을 때 실행할 동작들을 등록하는 대리자*/
	FOnStartMainGame OnStartMainGame;

	/* 매치가 시작되었을 때, 클라이언트 측에서 등록된 대리자를 발동 */
	UFUNCTION(Client, Reliable)
	void ClientRPCStartMainGame();

	/* 매치가 종료되었을 때, 클라이언트 측에서 등록된 대리자를 발동 */
	UFUNCTION(Client, Reliable)
	void ClientRPCSetupEndMatch();

	/* 매치 종료 후 일정 시간이 지났을 때, 서버가 클라이언트의 '로비로 이동'을 강제 발동 */
	UFUNCTION(Client, Reliable)
	void ClientRPCEndMatch();

	/* 매치가 종료되었을 때, 클라이언트 측에서 실행할 동작 */
	void SetupEndMatch();
	

// 서버 시간 동기화
public:
	/* 서버-클라이언트 시간 동기화를 통해 얻은 서버 시간을 반환한다 */
	float GetServerTime() const { return ServerTime; }

protected:
	/* 클라이언트에게 현재 서버의 시간과 클라이언트가 요청한 시간을 반환한다 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestServerTime(APlayerController* Requestor, float RequestWorldTime);

	/* 서버로부터 클라이언트의 요청 시간과 서버 시간을 받는다. 이를 기반으로 동기화된 서버 시간을 계산한다 */
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float RequestWorldTime, float RequestServerTime);


protected:
	/* 서버-클라이언트간 동기화가 적용된 서버의 시간이다 */
	float ServerTime;

// 입력 
public:
	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditAnyWhere, Category = "Enhanced Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnyWhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> ShowingScoreBoard;

	UPROPERTY(EditAnyWhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> ShowingGameMenu;


// UI 관리
public:
	/* 플레이어 컨트롤러가 소유한 스코어보드 위젯의 정보 업데이트 */
	void UpdateScoreBoard();

	void HideGameMenu();

protected:
	void ShowScoreBoard();
	void HideScoreBoard();
	void ShowGameMenu();

protected:
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UUserWidget> MatchEndWidgetClass;

	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<UEPScoreBoardWidget> ScoreBoardWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEPGameMenuWidget> GameMenuWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEPBasicTextWidget> StartMainGameWidgetClass;

	UPROPERTY()
	TObjectPtr<UEPGameEndWidget> MatchEndWidget;

	UPROPERTY()
	TObjectPtr<UEPScoreBoardWidget> ScoreBoardWidget;

	UPROPERTY()
	TObjectPtr<UEPGameMenuWidget> GameMenuWidget;

	UPROPERTY()
	TObjectPtr<UEPBasicTextWidget> StartMainGameWidget;


// 게임 시작
private:
	/*
	* 레벨 이동 직후에는 UI에 표시할 플레이어 정보가 충분히 준비되지 않는다.
	* 따라서 레벨 이동 직후에는 UI를 숨기고, 매치가 시작되었을 때 UI를 표시하기 위한 플래그이다.
	*/
	uint8 bIsStartMainGame : 1;

	/* 매치가 시작되었을 때, 이를 알리는 UI가 유지되는 기간 */
	float StartMainGameWidgetHideDelay;
};
