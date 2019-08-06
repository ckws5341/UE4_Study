// Fill out your copyright notice in the Description page of Project Settings.

#include "studyCharacterWidget.h"
#include "StudyCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UstudyCharacterWidget::BindCharacterStat(UStudyCharacterStatComponent* NewCharacterStat)
{
	if (nullptr != NewCharacterStat)
	{
		CurrentCharacterStat = NewCharacterStat;
		NewCharacterStat->OnHPChanged.AddUObject(this, &UstudyCharacterWidget::UpdateHPWidget);
	}
}

void UstudyCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	if (nullptr != HPProgressBar)
		UpdateHPWidget();
}

void UstudyCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}




