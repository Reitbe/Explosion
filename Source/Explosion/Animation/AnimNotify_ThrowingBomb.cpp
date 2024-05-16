// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ThrowingBomb.h"
#include "Explosion/Character/EPCharacterBase.h"

void UAnimNotify_ThrowingBomb::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AEPCharacterBase* Character = Cast<AEPCharacterBase>(MeshComp->GetOwner());
	if (Character)
	{
		Character->OnThrowingBombDelegate.Broadcast();
	}
}
