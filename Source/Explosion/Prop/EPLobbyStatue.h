// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPLobbyStatue.generated.h"

UCLASS()
class EXPLOSION_API AEPLobbyStatue : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEPLobbyStatue();
	void SetVisibility(bool bVisible);
	void SetNameTag(FString Name);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UChildActorComponent> ChildActorComponent;
	TSubclassOf<class ACharacter> StatueCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;
	TObjectPtr<class UStaticMesh> StatuePedestal;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UWidgetComponent> WidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<class UEPNameTagWidget> NameTagWidgetClass;
	TObjectPtr<class UEPNameTagWidget> NameTagWidget;

};
