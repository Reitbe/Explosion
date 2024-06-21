// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EPBattleRoyalGameMode.generated.h"

UCLASS()
class EXPLOSION_API AEPBattleRoyalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public: 
	AEPBattleRoyalGameMode();

	virtual FTransform GetRandomStartTransform();
	virtual void OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn);

protected:
	TArray<TObjectPtr<class APlayerStart>> PlayerStartArray;
	
};
