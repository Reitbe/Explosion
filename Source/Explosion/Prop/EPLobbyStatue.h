// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPLobbyStatue.generated.h"

class UCapsuleComponent;
class UChildActorComponent;
class ACharacter;
class UStaticMeshComponent;
class UStaticMesh;
class UWidgetComponent;
class UEPNameTagWidget;

/**
* 로비 화면에서 볼 수 있는 석상. LobbyStatueManager에 의해 관리된다.
*/
UCLASS()
class EXPLOSION_API AEPLobbyStatue : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPLobbyStatue();

	/* 석상 요소들의 가시성 설정 */
	void SetVisibility(bool bVisible);

	/* 석상 상단의 이름 태그 설정 */
	void SetNameTag(FString Name);

protected:
	virtual void BeginPlay() override;

protected:	
	/* RootComponent로 사용하기 위한 캡슐 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	/* 캐릭터를 석상 액터에 추가하기 위한 자손 액터 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UChildActorComponent> ChildActorComponent;

	/* 석상으로 사용할 캐릭터. 애니메이션을 적용할 수 있기에 StaticMesh가 아닌 Character 사용. */
	TSubclassOf<ACharacter> StatueCharacter;

	/* 석상의 밑단을 추가하기 위한 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	/* 석상의 밑단으로 사용할 스태틱 메쉬 */
	TObjectPtr<UStaticMesh> StatuePedestal;

	/* 석상에 위젯을 추가하기 위한 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	/* 석상 상단에 추가할 이름표 위젯 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<UEPNameTagWidget> NameTagWidgetClass;
	TObjectPtr<UEPNameTagWidget> NameTagWidget;

};
