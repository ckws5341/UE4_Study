// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "studyUnreal.h"
#include "StudyGameInstance.h"
#include "Components/ActorComponent.h"
#include "StudyCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STUDYUNREAL_API UStudyCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStudyCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	void SetNewLevel(int32 NewLevel);
	void SetDamage(float NewDamage);
	float GetAttack();
	void SetHP(float NewHP);
	float GetHPRatio();
	int32 GetDropExp() const;

	FOnHPChangedDelegate OnHPChanged;
	FOnHPIsZeroDelegate OnHPIsZero;

private:
	struct FStudyCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true)) int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true)) float CurrentHP;

};
