// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EPLobbyCharacter.generated.h"

/*
* 로비 캐릭터는 메인화면과 로비에서 사용되는 기본 캐릭터이다.
* UI 상호작용 이외에 특별한 기능을 가지지 않으며 이동이 불가능하다.
*/
UCLASS()
class EXPLOSION_API AEPLobbyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEPLobbyCharacter();

protected:
	virtual void BeginPlay() override;

};
