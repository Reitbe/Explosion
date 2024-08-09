// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EPLobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSION_API AEPLobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AEPLobbyPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool GetIsReady() const { return bIsReady; }
	void SetIsReady(bool bReady) { bIsReady = bReady; }

private:
	UPROPERTY(Replicated)
	uint8 bIsReady : 1;
	
};
