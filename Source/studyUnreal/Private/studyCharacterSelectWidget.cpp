// Fill out your copyright notice in the Description page of Project Settings.

#include "studyCharacterSelectWidget.h"
#include "studyCharacterSetting.h"
#include "StudyGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "studySaveGame.h"
#include "studyPlayerState.h"

void UstudyCharacterSelectWidget::NextCharacter(bool bForward)
{
	bForward ? CurrentIndex++ : CurrentIndex;

	if (CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;

	auto CharacterSetting = GetDefault<UstudyCharacterSetting>();
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	auto studyGameInstance = GetWorld()->GetGameInstance<UStudyGameInstance>();
	if (nullptr != studyGameInstance)
	{
		if (TargetComponent.IsValid())
		{
			USkeletalMesh* Asset = studyGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
			if (nullptr != Asset)
				TargetComponent->SetSkeletalMesh(Asset);
		}
	}
}

void UstudyCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	auto CharacterSetting = GetDefault<UstudyCharacterSetting>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	if (nullptr != PrevButton)
	{
		NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
		if (nullptr != NextButton)
		{
			TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
			if (nullptr != TextBox)
			{
				ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
				if (nullptr != ConfirmButton)
				{
					PrevButton->OnClicked.AddDynamic(this, &UstudyCharacterSelectWidget::OnPrevClicked);
					NextButton->OnClicked.AddDynamic(this, &UstudyCharacterSelectWidget::OnNextClicked);
					ConfirmButton->OnClicked.AddDynamic(this, &UstudyCharacterSelectWidget::OnConfirmClicked);
				}
			}
		}
	}
}

void UstudyCharacterSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}
void UstudyCharacterSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

void UstudyCharacterSelectWidget::OnConfirmClicked()
{
	FString CharacterName = TextBox->GetText().ToString();
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10) return;

	UstudySaveGame* NewPlayerData = NewObject<UstudySaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;
	auto studyPlayerState = GetDefault<AstudyPlayerState>();
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, studyPlayerState->SaveSlotName, 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Gameplay"));
	}
	else
		ABLOG(Error, TEXT("SaveGame Error"));
}




