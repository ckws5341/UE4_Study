// Fill out your copyright notice in the Description page of Project Settings.

#include "studyUIPlayerController.h"
#include "Blueprint/UserWidget.h"

void AstudyUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr != UIWidgetClass)
	{
		UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
		if (nullptr != UIWidgetInstance)
		{
			UIWidgetInstance->AddToViewport();

			FInputModeUIOnly Mode;
			Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
			SetInputMode(Mode);
			bShowMouseCursor = true;
		}
	}
}



