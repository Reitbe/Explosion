// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EPNameTagWidget.generated.h"

UCLASS()
class EXPLOSION_API UEPNameTagWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetNameTag(FString Name) { TxtNameTag->SetText(FText::FromString(Name)); }

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtNameTag;
};
