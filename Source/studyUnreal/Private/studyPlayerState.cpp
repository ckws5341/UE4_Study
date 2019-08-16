// Fill out your copyright notice in the Description page of Project Settings.

#include "studyPlayerState.h"
#include "studyGameInstance.h"
#include "studySaveGame.h"
AstudyPlayerState::AstudyPlayerState()
{
	GameHighScore = 0;
	SaveSlotName = TEXT("Player1");
	CharacterLevel = 1;
	GameScore = 0;
	Exp = 0;
	CharacterIndex = 0;

}

int32 AstudyPlayerState::GetGameScore() const
{
	return GameScore;
}
int32 AstudyPlayerState::GetGameHighScore() const
{
	return GameHighScore;
}
int32 AstudyPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}
int32 AstudyPlayerState::GetCharacterIndex() const
{
	return CharacterIndex;
}

void AstudyPlayerState::InitPlayerData()
{
	auto studySaveGame = Cast<UstudySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	if (nullptr == studySaveGame)
	{
		studySaveGame = GetMutableDefault<UstudySaveGame>();
	}
	SetPlayerName(studySaveGame->PlayerName);
	SetCharacterLevel(studySaveGame->Level);
	GameScore = 0;
	GameHighScore = studySaveGame->HighScore;
	Exp = studySaveGame->Exp;
	CharacterIndex = studySaveGame->CharacterIndex;
	SavePlayerData();
}
void AstudyPlayerState::SavePlayerData()
{
	UstudySaveGame* NewPlayerData = NewObject<UstudySaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;
	NewPlayerData->CharacterIndex = CharacterIndex;

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
		ABLOG(Error, TEXT("SaveGAmeError!"));
}

float AstudyPlayerState::GetExpRatio() const
{
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
		return 0.f;

	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	ABLOG(Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), Result, Exp, CurrentStatData->NextExp);
	return Result;
}
bool AstudyPlayerState::AddExp(int32 IncomeExp)
{
	if (CurrentStatData->NextExp == -1)
		return false;

	bool DidLevelUp = false;
	Exp = Exp + IncomeExp;

	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();

	return DidLevelUp;
}
void AstudyPlayerState::AddGameScore()
{
	GameScore++;
	if (GameScore >= GameHighScore)
		GameHighScore = GameScore;
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
}

void AstudyPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto studyGameInstance = Cast<UStudyGameInstance>(GetGameInstance());
	if (nullptr != studyGameInstance)
	{
		CurrentStatData = studyGameInstance->GetStudyCharacterData(NewCharacterLevel);
		if (nullptr != CurrentStatData)
			CharacterLevel = NewCharacterLevel;
	}
}

