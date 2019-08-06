// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "studyUnreal.h"
#include "AIController.h"
#include "studyAIController.generated.h"

/**
 * 
 */
UCLASS()
class STUDYUNREAL_API AstudyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AstudyAIController();
	virtual void Possess(APawn* InPawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

private :
	UPROPERTY() class UBehaviorTree * BTAsset;

	UPROPERTY() class UBlackboardData * BBAsset;
};
