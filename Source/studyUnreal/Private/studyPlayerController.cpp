// Fill out your copyright notice in the Description page of Project Settings.

#include "studyPlayerController.h"
#include "studyHUDWidget.h"
#include "studyPlayerState.h"
#include "studyCharacter.h"
#include "studyGameplayWidget.h"
#include "studyGameplayResultWidget.h"
#include "studyGameState.h"

AstudyPlayerController::AstudyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UstudyHUDWidget> UI_HUD_C(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
		HUDWidgetClass = UI_HUD_C.Class;

	static ConstructorHelpers::FClassFinder<UstudyGameplayWidget> UI_MENU_C(TEXT("/Game/Book/UI/UI_Menu.UI_Menu_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UstudyGameplayResultWidget> UI_RESULT_C(TEXT("/Game/Book/UI/UI_Result.UI_Result_C"));
	if (UI_RESULT_C.Succeeded())
	{
		ResultWidgetClass = UI_RESULT_C.Class;
	}
}
void AstudyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AstudyPlayerController::OnGamePause);
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

	ChangeInputMode(true);

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	HUDWidget = CreateWidget<UstudyHUDWidget>(this, HUDWidgetClass);
	if (nullptr != HUDWidget)
	{
		HUDWidget->AddToViewport();
		ResultWidget = CreateWidget<UstudyGameplayResultWidget>(this, ResultWidgetClass);
		studyPlayerState = Cast<AstudyPlayerState>(PlayerState);
		if (nullptr != studyPlayerState)
		{
			HUDWidget->BindPlayerState(studyPlayerState);
			studyPlayerState->OnPlayerStateChanged.Broadcast();
		}
	}
}
UstudyHUDWidget * AstudyPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}
void AstudyPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void AstudyPlayerController::NPCKill(AstudyCharacter* KilledNPC) const
{
	studyPlayerState->AddExp(KilledNPC->GetExp());
}

void AstudyPlayerController::AddGameScore() const
{
	studyPlayerState->AddGameScore();
}

void AstudyPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<UstudyGameplayWidget>(this, MenuWidgetClass);
	if (nullptr != MenuWidget)
	{
		MenuWidget->AddToViewport(3);
		SetPause(true);
		ChangeInputMode(false);
	}
}

void AstudyPlayerController::ShowResultUI()
{
	auto studyGameState = Cast<AstudyGameState>(UGameplayStatics::GetGameState(this));
	if (nullptr != studyGameState)
		ResultWidget->BindGameState(studyGameState);
	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}
