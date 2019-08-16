// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "studyUnreal.h"
#include "studyGameplayWidget.h"
#include "studyGameplayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class STUDYUNREAL_API UstudyGameplayResultWidget : public UstudyGameplayWidget
{
	GENERATED_BODY()
public :
	void BindGameState(class AstudyGameState* GameState);

protected :
	virtual void NativeConstruct() override;
	
private :
	TWeakObjectPtr<class AstudyGameState> CurrentGameState;
	
};
