// Fill out your copyright notice in the Description page of Project Settings.

#include "studyPlayerState.h"
#include "studyGameInstance.h"

AstudyPlayerState::AstudyPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	Exp = 0;
}

int32 AstudyPlayerState::GetGameScore() const
{
	return GameScore;
}
int32 AstudyPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}
void AstudyPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("Destiny"));
	SetCharacterLevel(5);
	Exp = 0;
	GameScore = 0;
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
	return DidLevelUp;
}
void AstudyPlayerState::AddGameScore()
{
	GameScore++;
	OnPlayerStateChanged.Broadcast();
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

