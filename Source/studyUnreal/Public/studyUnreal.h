// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class ECharacterState :uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};

DECLARE_LOG_CATEGORY_EXTERN(studyUnreal, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(")+FString::FromInt(__LINE__)+TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(studyUnreal, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(studyUnreal, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))