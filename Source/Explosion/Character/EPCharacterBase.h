// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EPCharacterBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnThrowingBombDelegate);

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

// Bomb Section.
protected:
	UPROPERTY(EditAnywhere, Category = "Bomb", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AEPBombBase> BP_Bomb;

	virtual void OnThrowingBomb();
	virtual void OnReloadingBomb();

// UI
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> WidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEPNameTagWidget> NameTagWidget;

};
