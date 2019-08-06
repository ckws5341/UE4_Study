// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "studyAIController.h"
#include "studyCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;

}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto studyCharacter = Cast<AstudyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == studyCharacter)
		return EBTNodeResult::Failed;

	studyCharacter->Attack();
	IsAttacking = true;
	studyCharacter->OnAttackEnd.AddLambda([this]()->void {
		IsAttacking = false;
	});


	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	if (!IsAttacking)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

