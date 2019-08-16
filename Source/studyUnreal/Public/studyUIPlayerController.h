// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "studyUnreal.h"
#include "GameFramework/PlayerController.h"
#include "studyUIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STUDYUNREAL_API AstudyUIPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI) TSubclassOf<class UUserWidget> UIWidgetClass;
	UPROPERTY() class UUserWidget* UIWidgetInstance;
	
	
};
