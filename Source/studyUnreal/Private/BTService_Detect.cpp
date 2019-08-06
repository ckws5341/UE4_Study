// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect.h"
#include "studyAIController.h"
#include "studyCharacter.h"
#include "BehaviorTree//BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn*ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);
	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			AstudyCharacter* studyCharacter = Cast<AstudyCharacter>(OverlapResult.GetActor());
			if (studyCharacter && studyCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AstudyAIController::TargetKey, studyCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.5f);
				DrawDebugPoint(World, studyCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.5f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), studyCharacter->GetActorLocation(), FColor::Blue, false, 0.5f);
				return;
			}
		}
	}
	else
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AstudyAIController::TargetKey, nullptr);

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}


