// Fill out your copyright notice in the Description page of Project Settings.

#include "studyPawn.h"


// Sets default values
AstudyPawn::AstudyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AstudyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AstudyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AstudyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AstudyPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AstudyPawn::PossessedBy(AController* NewController)
{
	ABLOG_S(Warning);
	Super::PossessedBy(NewController);
}
