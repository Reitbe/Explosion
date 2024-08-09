// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EPPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSetupEndMatch)
DECLARE_MULTICAST_DELEGATE(FOnStartMainGame)

UCLASS()
class EXPLOSION_API AEPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEPPlayerController();
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void StartMainGame();

// UI 표시
public:
	FOnSetupEndMatch OnSetupEndMatch;
	FOnStartMainGame OnStartMainGame;

	UFUNCTION(Client, Reliable)
	void ClientRPC_StartMainGame();

	UFUNCTION(Client, Reliable)
	void ClientRPCSetupEndMatch();

	void SetupEndMatch();

	UFUNCTION(Client, Reliable)
	void ClientRPCEndMatch();

	//UFUNCTION(Client, Reliable)
	//void ClientRPCUpdateScoreBoard();

	void UpdateScoreBoard();

	void HideGameMenu();

protected:
	void ShowScoreBoard();
	void HideScoreBoard();
	void ShowGameMenu();
	

// 서버 시간 동기화
public:
	float GetServerTime() const { return ServerTime; }

protected:
	virtual void ReceivedPlayer() override;

	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float requestWorldTime, float serverTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestServerTime(APlayerController* requestor, float requestWorldTime);

// 입력 
public:
	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditAnyWhere, Category = "Enhanced Input")
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnyWhere, Category = "Enhanced Input")
	TObjectPtr<class UInputAction> ShowingScoreBoard;

	UPROPERTY(EditAnyWhere, Category = "Enhanced Input")
	TObjectPtr<class UInputAction> ShowingGameMenu;


// UI 
protected:
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<class UUserWidget> MatchEndWidgetClass;

	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<class UEPScoreBoardWidget> ScoreBoardWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UEPGameMenuWidget> GameMenuWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UEPBasicTextWidget> StartMainGameWidgetClass;

	UPROPERTY()
	TObjectPtr<class UEPGameEndWidget> MatchEndWidget;

	UPROPERTY()
	TObjectPtr<class UEPScoreBoardWidget> ScoreBoardWidget;

	UPROPERTY()
	TObjectPtr<class UEPGameMenuWidget> GameMenuWidget;

	UPROPERTY()
	TObjectPtr<class UEPBasicTextWidget> StartMainGameWidget;

protected:
	float ServerTime;


private:
	uint8 bIsStartMainGame : 1;
};
