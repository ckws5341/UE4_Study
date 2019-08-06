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

	AstudyGameMode();

public :
	virtual void PostLogin(APlayerController * NewPlayer) override;
	
};
