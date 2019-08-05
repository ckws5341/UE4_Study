// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "studyUnreal.h"
#include "Blueprint/UserWidget.h"
#include "studyCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class STUDYUNREAL_API UstudyCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	void BindCharacterStat(class UStudyCharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private :
	TWeakObjectPtr<class UStudyCharacterStatComponent> CurrentCharacterStat;
	
	UPROPERTY() class UProgressBar* HPProgressBar;

};
