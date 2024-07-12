// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EPPlayerController.generated.h"


UCLASS()
class EXPLOSION_API AEPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEPPlayerController();
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;

	float GetServerTime() const { return ServerTime; }

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCSetupEndMatch();


protected:
	void ShowScoreBoard();
	void HideScoreBoard();
	void ShowGameMenu();

protected:
	virtual void ReceivedPlayer() override;
	

	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float requestWorldTime, float serverTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestServerTime(APlayerController* requestor, float requestWorldTime);
	

protected:
	UPROPERTY(EditAnyWhere, Category = "Enhanced Input")
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnyWhere, Category = "Enhanced Input")
	TObjectPtr<class UInputAction> ShowingScoreBoard;

	UPROPERTY(EditAnyWhere, Category = "Enhanced Input")
	TObjectPtr<class UInputAction> ShowingGameMenu;


protected:
	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<class UUserWidget> MatchEndWidgetClass;

	UPROPERTY(EditAnyWhere, Category = "UI")
	TSubclassOf<class UEPScoreBoardWidget> ScoreBoardWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UEPGameMenuWidget> GameMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<class UUserWidget> MatchEndWidget;
	UPROPERTY()
	TObjectPtr<class UEPScoreBoardWidget> ScoreBoardWidget;
	UPROPERTY()
	TObjectPtr<class UEPGameMenuWidget> GameMenuWidget;

protected:
	float ServerTime;
};
