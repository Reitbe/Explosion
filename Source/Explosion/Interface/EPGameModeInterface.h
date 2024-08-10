// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EPGameModeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEPGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EXPLOSION_API IEPGameModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FTransform GetRandomStartTransform() = 0;
	virtual void OnPlayerKilled(class AController* KillerPlayer, class AController* KilledPlayer, class AActor* DamageCauser) = 0;
};
