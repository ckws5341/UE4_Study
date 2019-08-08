// Fill out your copyright notice in the Description page of Project Settings.

#include "studyPlayerController.h"
#include "studyHUDWidget.h"
#include "studyPlayerState.h"
#include "studyCharacter.h"

AstudyPlayerController::AstudyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UstudyHUDWidget> UI_HUD_C(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
		HUDWidgetClass = UI_HUD_C.Class;
}

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
void AstudyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	HUDWidget = CreateWidget<UstudyHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();

	studyPlayerState = Cast<AstudyPlayerState>(PlayerState);
	if (nullptr != studyPlayerState)
	{
		HUDWidget->BindPlayerState(studyPlayerState);
		studyPlayerState->OnPlayerStateChanged.Broadcast();
	}
}
UstudyHUDWidget * AstudyPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AstudyPlayerController::NPCKill(AstudyCharacter* KilledNPC) const
{
	studyPlayerState->AddExp(KilledNPC->GetExp());
}

void AstudyPlayerController::AddGameScore() const
{
	studyPlayerState->AddGameScore();
}
