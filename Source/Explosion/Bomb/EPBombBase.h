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

// Set & Get Section
public:
	void SetBombOnwer(TSubclassOf<class AEPCharacterBase> _BombOwner) { BombOwner = _BombOwner; }

	TObjectPtr<class UStaticMeshComponent> GetBombMeshComponent() const { return BombMeshComponent; }
	float GetBombMass() const { return BombMass; }

// Replicate Section
protected:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCExplode();


// Component Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AEPCharacterBase> BombOwner;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UCapsuleComponent> BombCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> BombMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> BombMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAudioComponent> BombAudioComponent;

// Bomb Function Section
public:
	void ActiveBomb();
	void DeactiveBomb();
	void ActiveBombTimeTrigger();


// Bomb Particle Section
protected:
	UPROPERTY()
	TObjectPtr<class UGameplayStatics> BombGamePlayStatics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystem> BombParticleSystem;


// Bomb Stat(임시)
protected:
	UPROPERTY(VisibleAnyWhere, Category = "BombStat")
	float BombMass;

	UPROPERTY(VisibleAnyWhere, Category = "BombStat")
	float BombDamage;

	UPROPERTY(VisibleAnyWhere, Category = "BombStat")
	float BombArea;

	UPROPERTY(VisibleAnyWhere, Category = "BombStat")
	float BombDelayTime;

};
