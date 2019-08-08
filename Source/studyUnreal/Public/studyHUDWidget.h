// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "studyUnreal.h"
#include "Blueprint/UserWidget.h"
#include "studyHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class STUDYUNREAL_API UstudyHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UStudyCharacterStatComponent* CharacterStat);
	void BindPlayerState(class AstudyPlayerState* PlayerState);
	
protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private:
	TWeakObjectPtr<class UStudyCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class AstudyPlayerState> CurrentPlayerState;

	UPROPERTY() class UProgressBar* HPBar;
	UPROPERTY() class UProgressBar* ExpBar;
	UPROPERTY() class UTextBlock* PlayerName;
	UPROPERTY() class UTextBlock* PlayerLevel;
	UPROPERTY() class UTextBlock* CurrentScore;
	UPROPERTY() class UTextBlock* HighScore;

	
	
};
