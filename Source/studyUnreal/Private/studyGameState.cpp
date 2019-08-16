// Fill out your copyright notice in the Description page of Project Settings.

#include "studyGameState.h"

AstudyGameState::AstudyGameState()
{
	TotalGameScore = 0;
	bGameCleared = false;
}

int32 AstudyGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void AstudyGameState::AddGameScore()
{
	TotalGameScore++;
}

void AstudyGameState::SetGameCleared()
{
	bGameCleared = true;
}

bool AstudyGameState::IsGameCleared() const
{
	return bGameCleared;
}
