// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ThrowingBomb.h"
#include "Explosion/Character/EPCharacterBase.h"

void UAnimNotify_ThrowingBomb::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("노티파이 실행"));

	AEPCharacterBase* Character = Cast<AEPCharacterBase>(MeshComp->GetOwner());
	if (Character)
	{
		Character->OnThrowingBombDelegate.Broadcast();
	}
}
