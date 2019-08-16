// Fill out your copyright notice in the Description page of Project Settings.

#include "studyGameMode.h"
#include "studyPawn.h"
#include "studyCharacter.h"
#include "studyPlayerController.h"
#include "studyPlayerState.h"
#include "studyGameState.h"
AstudyGameMode::AstudyGameMode()
{
	DefaultPawnClass = AstudyCharacter::StaticClass();
	PlayerControllerClass = AstudyPlayerController::StaticClass();
	PlayerStateClass = AstudyPlayerState::StaticClass();
	GameStateClass = AstudyGameState::StaticClass();

	ScoreToClear = 2;
}

void AstudyGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	studyGameState = Cast<AstudyGameState>(GameState);
}

void AstudyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto studyPlayerState = Cast<AstudyPlayerState>(NewPlayer->PlayerState);
	if (nullptr != studyPlayerState)
		studyPlayerState->InitPlayerData();
}

void AstudyGameMode::AddScore(AstudyPlayerController* ScoredPlayer)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto studyPlayerController = Cast<AstudyPlayerController>(It->Get());
		if ((nullptr != studyPlayerController) && (ScoredPlayer == studyPlayerController))
		{
			studyPlayerController->AddGameScore();
			break;
		}
	}
	studyGameState->AddGameScore();

	if (GetScore() >= ScoreToClear)
	{
		studyGameState->SetGameCleared();

		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			(*It)->TurnOff();
		}
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			const auto studyPlayerController = Cast<AstudyPlayerController>(It->Get());
			if (nullptr != studyPlayerController)
				studyPlayerController->ShowResultUI();
		}
	}
}

int32 AstudyGameMode::GetScore() const
{
	return studyGameState->GetTotalGameScore();
}




