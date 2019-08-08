// Fill out your copyright notice in the Description page of Project Settings.

#include "studyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AstudyAIController::HomePosKey(TEXT("HomePos"));
const FName AstudyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AstudyAIController::TargetKey(TEXT("Target"));

AstudyAIController::AstudyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Book/AI/BB_studyCharacter.BB_studyCharacter"));
	if (BBObject.Succeeded())
		BBAsset = BBObject.Object;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Book/AI/BT_studyCharacter.BT_studyCharacter"));
	if (BTObject.Succeeded())
		BTAsset = BTObject.Object;
}



void AstudyAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
}

void AstudyAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
			ABLOG(Error, TEXT("AIController couldn't run behavior tree!"));
	}
}
void AstudyAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}


