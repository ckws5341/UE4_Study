// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "studyCharacterSetting.generated.h"

/**
 * 
 */
UCLASS(config=StudyUnreal)
class ARENABATTLESETTING_API UstudyCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public :
	UstudyCharacterSetting();

	UPROPERTY(config) TArray<FSoftObjectPath> CharacterAssets;

	
};
