// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "studyUnreal.h"
#include "GameFramework/GameModeBase.h"
#include "studyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STUDYUNREAL_API AstudyGameMode : public AGameModeBase
{
	GENERATED_BODY()


public :
	AstudyGameMode();

	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController * NewPlayer) override;
	void AddScore(class AstudyPlayerController * Player);

private :
	UPROPERTY() class AstudyGameState* studyGameState;

};
