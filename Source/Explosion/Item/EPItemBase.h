// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion/EPCharacterStat.h"
#include "EPItemBase.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;
class UParticleSystemComponent;

/*
* 모든 아이템들의 기본이 되는 클래스. 
* 캡슐 콜라이더와 충돌했을 때 아이템 효과가 발동된다.
* 지금은 체력 회복 타입만 구현되어 있다.
*/

/* 아이템 타입 구분을 위한 enum */
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
	AEPItemBase();
	
protected:
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

	/* 아이템이 가지는 스탯. 캐릭터에 적용하기 쉽도록 캐릭터의 스탯과 동일한 타입을 사용. */
	FEPCharacterStat ItemStat;

};
