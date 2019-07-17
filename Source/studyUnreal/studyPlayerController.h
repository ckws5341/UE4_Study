// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "studyUnreal.h"
#include "GameFramework/PlayerController.h"
#include "studyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STUDYUNREAL_API AstudyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializeComponents() override;
	virtual void Possess(APawn* aPawn) override;

	virtual void BeginPlay() override;
	
};
