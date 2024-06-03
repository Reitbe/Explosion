// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EPCharacterBase.generated.h"

DECLARE_DELEGATE(FOnThrowingBombDelegate);
DECLARE_DELEGATE(FOnReloadingBombDelegate);

UCLASS()
class EXPLOSION_API AEPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEPCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


// Delegate Section
public:
	FOnThrowingBombDelegate OnThrowingBombDelegate;
	FOnReloadingBombDelegate OnReloadingBombDelegate;

// Bomb Manager Section.
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BombManager", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEPBombManager> BombManager;

// Bomb Section
protected:
	UPROPERTY(EditAnywhere, Category = "Bomb", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AEPBombBase> BP_Bomb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", Meta = (AllowPrivateAccess = "true"))
	float ThrowingVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", Meta = (AllowPrivateAccess = "true"))
	float BombMass;

	virtual void OnThrowingBomb();
	virtual void OnReloadingBomb();

// UI
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> WidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEPNameTagWidget> NameTagWidget;

};
