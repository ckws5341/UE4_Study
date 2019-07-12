// Fill out your copyright notice in the Description page of Project Settings.

#include "studyPlayerController.h"

void AstudyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}
void AstudyPlayerController::Possess(APawn * aPawn)
{
	ABLOG_S(Warning);
	Super::Possess(aPawn);
}



