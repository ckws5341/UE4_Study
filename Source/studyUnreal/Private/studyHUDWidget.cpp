// Fill out your copyright notice in the Description page of Project Settings.

#include "studyHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "StudyCharacterStatComponent.h"
#include "studyPlayerState.h"

void UstudyHUDWidget::BindCharacterStat(UStudyCharacterStatComponent * CharacterStat)
{
	if (nullptr != CharacterStat)
	{
		CurrentCharacterStat = CharacterStat;
		CharacterStat->OnHPChanged.AddUObject(this, &UstudyHUDWidget::UpdateCharacterStat);
	}
}

void UstudyHUDWidget::BindPlayerState(AstudyPlayerState* PlayerState)
{
	if (nullptr != PlayerState)
	{
		CurrentPlayerState = PlayerState;
		PlayerState->OnPlayerStateChanged.AddUObject(this, &UstudyHUDWidget::UpdatePlayerState);
	}
}

void UstudyHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	if (nullptr == HPBar)
		return;

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	if (nullptr == ExpBar)
		return;

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	if (nullptr == PlayerName)
		return;

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	if (nullptr == PlayerLevel)
		return;

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	if (nullptr == CurrentScore)
		return;

	HighScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	if (nullptr == HighScore)
		return;

}

void UstudyHUDWidget::UpdateCharacterStat()
{
	if (CurrentCharacterStat.IsValid())
	{
		HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
	}
}

void UstudyHUDWidget::UpdatePlayerState()
{
	if (CurrentPlayerState.IsValid())
	{
		ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
		PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
		PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
		CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));
		HighScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameHighScore())));
	}
}



