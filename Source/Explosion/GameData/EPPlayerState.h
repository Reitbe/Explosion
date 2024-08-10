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
	AEPPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//virtual FString GetPlayerNameCustom() const override;

public:	
	int32 GetKillCount() { return KillCount; };
	int32 GetDeathCount() { return DeathCount; };
	void AddKillCount() { ++KillCount; };
	void AddDeathCount() { ++DeathCount; };

	void SetIsKillCountReplicated(bool bIsReplicated) { bKillCountReplicated = bIsReplicated; };
	void SetIsDeathCountReplicated(bool bIsReplicated) { bDeathCountReplicated = bIsReplicated; };
	bool GetIsKillCountReplicated() const { return bKillCountReplicated; };
	bool GetIsDeathCountReplicated() const { return bDeathCountReplicated; };

	UFUNCTION()
	void OnRep_UpdateKillCount();
	
	UFUNCTION()
	void OnRep_UpdateDeathCount();

private:
	//UPROPERTY(Replicated)
	UPROPERTY(ReplicatedUsing = OnRep_UpdateKillCount)
	int32 KillCount;

	//UPROPERTY(Replicated)
	UPROPERTY(ReplicatedUsing = OnRep_UpdateDeathCount)
	int32 DeathCount;

private: 
	uint8 bKillCountReplicated : 1;
	uint8 bDeathCountReplicated : 1;
};