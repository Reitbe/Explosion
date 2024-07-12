// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EPBombBase.h"
#include "EPBombManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXPLOSION_API UEPBombManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEPBombManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

// Bomb Manager Function Section
public:
	void MakeBombObjectPool(TSubclassOf<AEPBombBase> BP_Bomb);
	
	TObjectPtr<AEPBombBase> TakeBomb();

// Bomb Manager Properties Section
private:
	TArray<TObjectPtr<AEPBombBase>> BombList;

	UPROPERTY(VisibleAnyWhere, Category = "Properties")
	int32 MaxBombCount;

	UPROPERTY(VisibleAnyWhere, Category = "Properties")
	int32 CurrentBombCount;

};
