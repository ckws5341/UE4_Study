// Fill out your copyright notice in the Description page of Project Settings.

#include "studyGameplayWidget.h"
#include "Components/Button.h"
#include "studyPlayerController.h"


void UstudyGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UstudyGameplayWidget::OnResumeClicked);
	}
	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UstudyGameplayWidget::OnReturnToTitleClicked);
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &UstudyGameplayWidget::OnRetryGameClicked);
	}
}

void UstudyGameplayWidget::OnResumeClicked()
{
	auto studyPlayerController = Cast<AstudyPlayerController>(GetOwningPlayer());
	if (nullptr != studyPlayerController)
	{
		RemoveFromParent();
		studyPlayerController->ChangeInputMode(true);
		studyPlayerController->SetPause(false);
	}
}
void UstudyGameplayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}
void UstudyGameplayWidget::OnRetryGameClicked()
{
	auto studyPlayerController = Cast<AstudyPlayerController>(GetOwningPlayer());
	if (nullptr != studyPlayerController)
		studyPlayerController->RestartLevel();
}



