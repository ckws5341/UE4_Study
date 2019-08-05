// Fill out your copyright notice in the Description page of Project Settings.

#include "StudyGameInstance.h"

UStudyGameInstance::UStudyGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/Book/GameData/StudyCharacterData.StudyCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_studyCharacter(*CharacterDataPath);
	StudyCharacterTable = DT_studyCharacter.Object;
}
void UStudyGameInstance::Init()
{
	Super::Init();
	ABLOG(Warning, TEXT("DropExp of Level 20 StudyCharacter : %d"), GetStudyCharacterData(20)->DropExp);
}

FStudyCharacterData * UStudyGameInstance::GetStudyCharacterData(int32 Level)
{
	return StudyCharacterTable->FindRow<FStudyCharacterData>(*FString::FromInt(Level), TEXT(""));
}


