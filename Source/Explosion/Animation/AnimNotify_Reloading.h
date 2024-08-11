// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Reloading.generated.h"


UCLASS()
class EXPLOSION_API UAnimNotify_Reloading : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	/** 던지기 몽타주 재생 중 재장전 시점에 실행되는 알림 */
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
};
