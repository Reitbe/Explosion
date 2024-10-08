// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Reloading.h"
#include "Explosion/Character/EPCharacterBase.h"

void UAnimNotify_Reloading::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AEPCharacterBase* EPCharacter = Cast<AEPCharacterBase>(MeshComp->GetOwner());
	if (EPCharacter)
	{
		EPCharacter->OnReloadingBombDelegate.Execute();
	}
}
