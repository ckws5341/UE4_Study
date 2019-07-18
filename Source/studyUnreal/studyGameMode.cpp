// Fill out your copyright notice in the Description page of Project Settings.

#include "studyGameMode.h"
#include "studyPawn.h"
#include "studyCharacter.h"
#include "studyPlayerController.h"

AstudyGameMode::AstudyGameMode()
{
	DefaultPawnClass = AstudyCharacter::StaticClass();
	PlayerControllerClass = AstudyPlayerController::StaticClass();
}

void AstudyGameMode::PostLogin(APlayerController* NewPlayer)
{
	ABLOG(Warning, TEXT("Postlogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("Postlogin End"));
}





