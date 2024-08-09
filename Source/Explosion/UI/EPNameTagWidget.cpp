// Fill out your copyright notice in the Description page of Project Settings.


#include "EPNameTagWidget.h"
#include "Components/TextBlock.h"

void UEPNameTagWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEPNameTagWidget::SetNameTag(FString Name)
{
	TxtNameTag->SetText(FText::FromString(Name));
}

void UEPNameTagWidget::ShowPlayerNetRole(TObjectPtr<APawn> Player)
{
	ENetRole NetRole = Player->GetLocalRole();

	FString Role;

	switch (NetRole)
	{
		case ENetRole::ROLE_Authority:
			Role = "Authority";
			break;
		case ENetRole::ROLE_AutonomousProxy:
			Role = "AutonomousProxy";
			break;
		case ENetRole::ROLE_SimulatedProxy:
			Role = "SimulatedProxy";
			break;
		case ENetRole::ROLE_None:
			Role = "None";
			break;
	}

	SetNameTag(Role);
}
