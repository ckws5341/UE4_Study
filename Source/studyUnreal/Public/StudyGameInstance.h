// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "studyUnreal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "StudyGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FStudyCharacterData : public FTableRowBase
{	   
	GENERATED_BODY()

public:
	FStudyCharacterData() : Level(1), MaxHP(100.f), Attack(10.f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") int32 DropExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") int32 NextExp;
};
/**
 * 
 */
UCLASS()
class STUDYUNREAL_API UStudyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UStudyGameInstance();

	virtual void Init() override;
	FStudyCharacterData* GetStudyCharacterData(int32 Level);

	FStreamableManager StreamableManager;
private :
	UPROPERTY() class UDataTable* StudyCharacterTable;
	
};
