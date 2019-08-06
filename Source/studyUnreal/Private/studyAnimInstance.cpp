// Fill out your copyright notice in the Description page of Project Settings.

#include "studyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
UstudyAnimInstance::UstudyAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	IsInAir = false;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> study_attackMontage(TEXT("/Game/Book/Animation/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (study_attackMontage.Succeeded())
		AttackMontage = study_attackMontage.Object;
}

void UstudyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	if(!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
			IsInAir = Character->GetMovementComponent()->IsFalling();
	}
}

void UstudyAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void UstudyAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UstudyAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UstudyAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}


FName UstudyAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}