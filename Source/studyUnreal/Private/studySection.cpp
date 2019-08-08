// Fill out your copyright notice in the Description page of Project Settings.

#include "studySection.h"
#include "studyCharacter.h"
#include "studyItemBox.h"
#include "studyPlayerController.h"
#include "studyGameMode.h"
// Sets default values
AstudySection::AstudySection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	FString AssetPath = TEXT("/Game/Book/StaticMesh/SM_SQUARE.SM_SQUARE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> study_Square(*AssetPath);
	if (study_Square.Succeeded())
		Mesh->SetStaticMesh(study_Square.Object);
	else
		ABLOG(Error, TEXT("Failed to load Static Mesh Asset : %s"), *AssetPath);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector(775.f, 775.f, 300.f));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector(0.f, 0.f, 250.f));
	Trigger->SetCollisionProfileName(TEXT("studyTrigger"));

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AstudySection::OnTriggerBeginOverlap);

	FString GateAssetPath = TEXT("/Game/Book/StaticMesh/SM_GATE.SM_GATE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> study_Gate(*GateAssetPath);
	if (!study_Gate.Succeeded())
		ABLOG(Error, TEXT("Failed to load static Mesh Asset : %s"), *GateAssetPath);

	static FName GateSockets[] = { {TEXT("+XGate")},{TEXT("-XGate")}, {TEXT("+YGate")}, {TEXT("-YGate")} };
	for (FName GateSocket : GateSockets)
	{
		if (Mesh->DoesSocketExist(GateSocket))
		{
			UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString());
			NewGate->SetStaticMesh(study_Gate.Object);
			NewGate->SetupAttachment(RootComponent, GateSocket);
			NewGate->SetRelativeLocation(FVector(0.f, -80.5f, 0.f));
			GateMeshes.Add(NewGate);
		}
		UBoxComponent* NewGateTrigger = CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString().Append(TEXT("Trigger")));
		NewGateTrigger->SetBoxExtent(FVector(100.f, 100.f, 300.f));
		NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
		NewGateTrigger->SetRelativeLocation(FVector(70.f, 0.f, 250.f));
		NewGateTrigger->SetCollisionProfileName(TEXT("studyTrigger"));
		GateTriggers.Add(NewGateTrigger);

		NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AstudySection::OnGateTriggerBeginOverlap);
		NewGateTrigger->ComponentTags.Add(GateSocket);
	}

	bNoBattle = false;
	EnemySpawnTime = 2.f;
	ItemBoxSpawnTime = 5.f;

}


// Called when the game starts or when spawned
void AstudySection::BeginPlay()
{
	Super::BeginPlay();
	
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

// Called every frame
void AstudySection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AstudySection::SetState(ESectionState NewState)
{
	switch (NewState)
	{
	case ESectionState::READY:
	{
		Trigger->SetCollisionProfileName(TEXT("studyTrigger"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(true);
		break;
	}
	case ESectionState::BATTLE:
	{
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(false);

		GetWorld()->GetTimerManager().SetTimer(SpawnNPCTimerHandle, FTimerDelegate::CreateUObject(this, &AstudySection::OnNPCSpawn), EnemySpawnTime, false);
		GetWorld()->GetTimerManager().SetTimer(SpawnItemBoxTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
			FVector2D RandXY = FMath::RandPointInCircle(600.f);
			GetWorld()->SpawnActor<AstudyItemBox>(GetActorLocation() + FVector(RandXY, 30.f), FRotator::ZeroRotator);
		}), ItemBoxSpawnTime, false);

		break;
	}
	case ESectionState::COMPLETE:
	{
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
			GateTrigger->SetCollisionProfileName(TEXT("studyTrigger"));
		OperateGates(true);
		CurrentState = ESectionState::READY;
		break;
	}
	}
	CurrentState = NewState;
}

void AstudySection::OperateGates(bool bOpen)
{
	for (UStaticMeshComponent* Gate : GateMeshes)
	{
		Gate->SetRelativeRotation(bOpen ? FRotator(0.f, -90.f, 0.f) : FRotator::ZeroRotator);
	}
}
void AstudySection::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

void AstudySection::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (CurrentState == ESectionState::READY)
		SetState(ESectionState::BATTLE);
}

void AstudySection::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent->ComponentTags.Num() == 1)
	{
		FName ComponentTag = OverlappedComponent->ComponentTags[0];
		FName SocketName = FName(*ComponentTag.ToString().Left(2));
		if (!Mesh->DoesSocketExist(SocketName))
			return;

		FVector NewLocation = Mesh->GetSocketLocation(SocketName);

		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
		FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);
		bool bResult = GetWorld()->OverlapMultiByObjectType(
			OverlapResults,
			NewLocation,
			FQuat::Identity,
			ObjectQueryParam,
			FCollisionShape::MakeSphere(775.f),
			CollisionQueryParam
		);
		if (!bResult)
			auto NewSection = GetWorld()->SpawnActor<AstudySection>(NewLocation, FRotator::ZeroRotator);
		else
			ABLOG(Warning, TEXT("New Section Area is not empty"));
	}
}

void AstudySection::OnNPCSpawn()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnNPCTimerHandle);
	auto KeyNPC = GetWorld()->SpawnActor<AstudyCharacter>(GetActorLocation() + FVector::UpVector * 88.f, FRotator::ZeroRotator);
	if (nullptr != KeyNPC)
		KeyNPC->OnDestroyed.AddDynamic(this, &AstudySection::OnKeyNPCDestroyed);
}

void AstudySection::OnKeyNPCDestroyed(AActor * DestroyedActor)
{
	auto studyCharacter = Cast<AstudyCharacter>(DestroyedActor);
	if (nullptr != studyCharacter)
	{
		auto studyPlayerController = Cast<AstudyPlayerController>(studyCharacter->LastHitBy);
		if (nullptr != studyPlayerController)
		{
			auto studyGameMode = Cast<AstudyGameMode>(GetWorld()->GetAuthGameMode());
			if (nullptr != studyGameMode)
				studyGameMode->AddScore(studyPlayerController);
			SetState(ESectionState::COMPLETE);
		}
	}
}