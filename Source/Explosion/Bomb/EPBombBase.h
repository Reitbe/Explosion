// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPBombBase.generated.h"

UCLASS()
class EXPLOSION_API AEPBombBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPBombBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void SetOnwerCharacter(TSubclassOf<class AEPCharacterBase> NewOwnerCharacter) { OwnerCharacter = NewOwnerCharacter; }

// Component Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AEPCharacterBase> OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> BombMesh;

// Notify Section
protected:
	UFUNCTION()
	void OnThrowingBomb();

};
