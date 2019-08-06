// Fill out your copyright notice in the Description page of Project Settings.

#include "studyWeapon.h"


// Sets default values
AstudyWeapon::AstudyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> study_Weapon(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	if (study_Weapon.Succeeded())
		Weapon->SetSkeletalMesh(study_Weapon.Object);

	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

}

// Called when the game starts or when spawned
void AstudyWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AstudyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

