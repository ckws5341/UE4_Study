// Fill out your copyright notice in the Description page of Project Settings.

#include "StudyCharacterStatComponent.h"

// Sets default values for this component's properties
UStudyCharacterStatComponent::UStudyCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UStudyCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStudyCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UStudyCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto StudyGameInstance = Cast<UStudyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if(nullptr != StudyGameInstance)
		CurrentStatData = StudyGameInstance->GetStudyCharacterData(NewLevel);

	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		ABLOG(Error, TEXT("Level (%d) Data dosen't exist"), NewLevel);
	}
}
void UStudyCharacterStatComponent::SetDamage(float NewDamage)
{
	if (nullptr != CurrentStatData)
	{
		SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.f, CurrentStatData->MaxHP));
	}
}

void UStudyCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.f;
		OnHPIsZero.Broadcast();
	}
}
float UStudyCharacterStatComponent::GetAttack()
{
	if (nullptr != CurrentStatData)
		return CurrentStatData->Attack;
	else return 0.f;
}

float UStudyCharacterStatComponent::GetHPRatio()
{
	if (nullptr != CurrentStatData)
		return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.f : (CurrentHP / CurrentStatData->MaxHP);
	else return 0.f;
}

int32 UStudyCharacterStatComponent::GetDropExp() const
{
	return CurrentStatData->DropExp;
}
