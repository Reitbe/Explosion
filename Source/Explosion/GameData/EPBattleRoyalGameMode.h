// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Explosion/Interface/EPGameModeInterface.h"
#include "EPBattleRoyalGameMode.generated.h"

UCLASS()
class EXPLOSION_API AEPBattleRoyalGameMode : public AGameModeBase, public IEPGameModeInterface
{
	GENERATED_BODY()

public: 
	AEPBattleRoyalGameMode();

	virtual FTransform GetRandomStartTransform() override;
	virtual void OnPlayerKilled(AController* KillerPlayer, AController* KilledPlayer, AActor* DamageCauser) override;

protected:
	TArray<TObjectPtr<class APlayerStart>> PlayerStartArray;
	
};
