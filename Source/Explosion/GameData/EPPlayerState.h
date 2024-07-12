// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EPPlayerState.generated.h"


UCLASS()
class EXPLOSION_API AEPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	int32 GetKillCount() { return KillCount; };
	int32 GetDeathCount() { return DeathCount; };
	void AddKillCount() { ++KillCount; };
	void AddDeathCount() { ++DeathCount; };
	
private:
	int32 KillCount;
	int32 DeathCount;
};