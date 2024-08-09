// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EPLobbyStatueManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXPLOSION_API UEPLobbyStatueManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEPLobbyStatueManager();
	void GetAllLobbyStatue();
	void UpdateLobbyStatue(bool IsOnMainMenu);

protected:
	TSubclassOf<class AEPLobbyStatue> LobbyStatueWithPedestalsClass;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> LobbyStatueWithPedestalsArray;

};
