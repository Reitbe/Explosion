// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion/EPCharacterStat.h"
#include "EPItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_Health = 0
};

UCLASS()
class EXPLOSION_API AEPItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEPItemBase();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	
	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Item Base")
	TObjectPtr<class UStaticMeshComponent> ItemMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Item Base")
	TObjectPtr<class UStaticMeshComponent> ItemBaseMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Item Base")
	TObjectPtr<class UCapsuleComponent> CapsuleTriggerComponent;

	UPROPERTY(VisibleAnywhere, Category = "Item Base")
	TObjectPtr<class UParticleSystemComponent> OverlapEffectComponent;


public:
	EItemType Type;
	FEPCharacterStat ItemStat;

};
