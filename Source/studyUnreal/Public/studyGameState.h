// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "studyUnreal.h"
#include "GameFramework/GameStateBase.h"
#include "studyGameState.generated.h"

/**
 * 
 */
UCLASS()
class STUDYUNREAL_API AstudyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public :
	AstudyGameState();

	int32 GetTotalGameScore() const;
	void AddGameScore();
	void SetGameCleared();
	bool IsGameCleared() const;

private :
	UPROPERTY(Transient) int32 TotalGameScore;
	UPROPERTY(Transient) bool bGameCleared;
	
};
