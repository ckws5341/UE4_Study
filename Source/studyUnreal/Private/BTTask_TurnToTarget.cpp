// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToTarget.h"
#include "studyAIController.h"
#include "studyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto studyCharacter = Cast<AstudyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == studyCharacter)
		return EBTNodeResult::Failed;

	auto Target = Cast<AstudyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AstudyAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - studyCharacter->GetActorLocation();
	LookVector.Z = 0.f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	studyCharacter->SetActorRotation(FMath::RInterpTo(studyCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.f));

	return EBTNodeResult::Succeeded;
}


