// Fill out your copyright notice in the Description page of Project Settings.

#include "Fountain.h"


// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
	Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));
	Movement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("MOVEMENT"));


	RootComponent = Body;
	Water->SetupAttachment(Body);
	Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);

	Water->SetRelativeLocation(FVector(0.f, 0.f, 135.f));
	Light->SetRelativeLocation(FVector(0.f, 0.f, 195.f));
	Splash->SetRelativeLocation(FVector(0.f, 0.f, 195.f));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_WATER(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02.SM_Plains_Fountain_02"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PS_SPLASH(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01.P_Water_Fountain_Splash_Base_01"));
	
	if (SM_BODY.Succeeded())
		Body->SetStaticMesh(SM_BODY.Object);
	if (SM_WATER.Succeeded())
		Water->SetStaticMesh(SM_WATER.Object);
	if(PS_SPLASH.Succeeded())
		Splash->SetTemplate(PS_SPLASH.Object);

	RotateSpeed = 30.f;
	Movement->RotationRate = FRotator(0.f, RotateSpeed, 0.f);
}

// Called when the game starts or when spawned
void AFountain::BeginPlay()
{
	Super::BeginPlay();
	
	ABLOG_S(Warning);
	ABLOG(Warning, TEXT("Actor Name : %s, ID : %d, Location X : %.3f"), *GetName(), ID, GetActorLocation().X);
}

// Called every frame
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AddActorLocalRotation(FRotator(0.f, RotateSpeed * DeltaTime, 0.f));
}

void AFountain::EndPlay(const EEndPlayReason::Type EndPlayReason)\
{
	Super::EndPlay(EndPlayReason);
	ABLOG_S(Warning);
}

void AFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}