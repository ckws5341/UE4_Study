// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "studySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class STUDYUNREAL_API UstudySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public :
	UstudySaveGame();
	UPROPERTY() int32 Level;
	UPROPERTY() int32 Exp;
	UPROPERTY() FString PlayerName;
	UPROPERTY() int32 HighScore;
	UPROPERTY() int32 CharacterIndex;


};
