// Fill out your copyright notice in the Description page of Project Settings.

#include "studyItemBox.h"
#include "studyWeapon.h"
#include "studyCharacter.h"

// Sets default values
AstudyItemBox::AstudyItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));


	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);


	Trigger->SetBoxExtent(FVector(40.f, 42.f, 30.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> studyBox(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (studyBox.Succeeded())
	{
		Box->SetStaticMesh(studyBox.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Boxopen(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (Boxopen.Succeeded())
	{
		Effect->SetTemplate(Boxopen.Object);
		Effect->bAutoActivate = false;
	}

	Box->SetRelativeLocation(FVector(0.f, -3.5f, -30.f));

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("ItemBox"));
	
	WeaponItemClass = AstudyWeapon::StaticClass();

}

// Called when the game starts or when spawned
void AstudyItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AstudyItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AstudyItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AstudyItemBox::OnCharacterOverlap);
}

void AstudyItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFrmpSweep, const FHitResult& SweepResult)
{
	ABLOG_S(Warning);

	auto studyCharacter = Cast<AstudyCharacter>(OtherActor);

	if (nullptr != studyCharacter && nullptr != WeaponItemClass)
	{
		if (studyCharacter->CanSetWeapon())
		{
			auto NewWeapon = GetWorld()->SpawnActor<AstudyWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			studyCharacter->SetWeapon(NewWeapon);
			Effect->Activate(true);
			Box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);
			Effect->OnSystemFinished.AddDynamic(this, &AstudyItemBox::OnEffectFinished);
		}
		else
		{
			ABLOG(Warning, TEXT("%s CAN'T EQUIP WEAPON CURRENTLY"), *studyCharacter->GetName());
		}
	}
}

void AstudyItemBox::OnEffectFinished(UParticleSystemComponent* Psystem)
{
	Destroy();
}
