// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Explosion/Interface/EPGameModeInterface.h"
#include "EPDeathMatchGameMode.generated.h"

UCLASS()
class EXPLOSION_API AEPDeathMatchGameMode : public AGameMode, public IEPGameModeInterface
{
	GENERATED_BODY()

public:
	AEPDeathMatchGameMode();
	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void StartPlay() override;

	virtual FTransform GetRandomStartTransform() override;
	FTransform GetRandomItemSpawnTransform();
	virtual void OnPlayerKilled(AController* KillerPlayer, AController* KilledPlayer, AActor* DamageCauser) override;

	int GetMatchScoreLimit() const { return MatchScoreLimit; }
	float GetMatchTimeLimit() const { return MatchTimeLimit; }

	void CheckAllPlayersReady();

protected:
	void SetTheEndMatch();
	void StartMainGame();
	void EndMatch();

	int32 GetLobbyPlayerCount() const;

protected:
	TSubclassOf<class APlayerStart> PlayerStartPointClass;
	TSubclassOf<class APlayerStart> ItemSpawnPointClass;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> PlayerStartPointsArray;
	TQueue<int32> OccupiedPlayerStartPointIndices;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> ItemSpawnPointsArray;

protected:
	int32 MatchScoreLimit;
	float MatchTimeLimit;

	int32 ReturnToLobbyDelay;

private:
	int32 ReadyPlayerCount{ 0 };
	int32 PlayerCountInGame{ 0 }; 
};
