// Fill out your copyright notice in the Description page of Project Settings.

#include "studyGameplayResultWidget.h"
#include "Components/TextBlock.h"
#include "studyGameState.h"

void UstudyGameplayResultWidget::BindGameState(AstudyGameState * GameState)
{
	if (nullptr != GameState)
		CurrentGameState = GameState;
}
void UstudyGameplayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (CurrentGameState.IsValid())
	{
		auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
		if (nullptr != Result)
		{
			Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ? TEXT("Mission Complete") : TEXT("Mission Failed")));
			auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
			if (nullptr != TotalScore)
			{
				TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));
			}
		}

	}
}




