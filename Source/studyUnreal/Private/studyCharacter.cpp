// Fill out your copyright notice in the Description page of Project Settings.

#include "studyCharacter.h"
#include "studyAnimInstance.h"
#include "studyWeapon.h"
#include "StudyCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "studyCharacterWidget.h"
#include "studyAIController.h"

// Sets default values
AstudyCharacter::AstudyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UStudyCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> studyMesh(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Bladed.SK_CharM_Bladed"));
	if (studyMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(studyMesh.Object);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> studyAnim(TEXT("/Game/Book/Animation/studyAnimBP.studyAnimBP_C"));
	if (studyAnim.Succeeded())
		GetMesh()->SetAnimInstanceClass(studyAnim.Class);

	SetControlMode(EControlMode::DIABLO);

	ArmLengthSpeed = 3.f;
	ArmRotationSpeed = 10.f;
	GetCharacterMovement()->JumpZVelocity = 800.f;

	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("studyCharacter"));

	AttackRange = 200.f;
	AttackRadius = 50.f;

	HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> study_UI(TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar_C"));
	if (study_UI.Succeeded())
	{
		HPBarWidget->SetWidgetClass(study_UI.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
	}

	AIControllerClass = AstudyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AstudyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AstudyCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}

void AstudyCharacter::SetWeapon(AstudyWeapon* NewWeapon)
{
	FName WeaponSocket(TEXT("Hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}
// Called every frame
void AstudyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		SpringArm->RelativeRotation = FMath::RInterpTo(SpringArm->RelativeRotation, ArmRotationTo, DeltaTime, ArmRotationSpeed);
		if (DirectionToMove.SizeSquared() > 0.f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
		break;
	}
}

// Called to bind functionality to input
void AstudyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AstudyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AstudyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AstudyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AstudyCharacter::Turn);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AstudyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AstudyCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AstudyCharacter::Attack);

}

void AstudyCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.X = NewAxisValue;
		break;
	}
}

void AstudyCharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.Y = NewAxisValue;
		break;
	}
}

void AstudyCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);
		break;
	}
}

void AstudyCharacter::Turn(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);
		break;
	}
}

void AstudyCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		//SpringArm->TargetArmLength = 450.f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengthTo = 450.f;

		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
		break;
	case EControlMode::DIABLO:
		//SpringArm->TargetArmLength = 800.f;
		//SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
		ArmLengthTo = 800.f;
		ArmRotationTo = FRotator(-45.f, 0.f, 0.f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
		break;
	case EControlMode::NPC :
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
		break;
	}
}

void AstudyCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->RelativeRotation);
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AstudyCharacter::Attack()
{
	if (IsAttacking)
	{
		if (CanNextCombo)
			IsComboInputOn = true;
	}
	else
	{
		AttackStartComboState();
		studyAnim->PlayAttackMontage();
		studyAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void AstudyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	studyAnim = Cast<UstudyAnimInstance>(GetMesh()->GetAnimInstance());
	
	studyAnim->OnMontageEnded.AddDynamic(this, &AstudyCharacter::OnAttackMontageEnded);

	studyAnim->OnNextAttackCheck.AddLambda([this]()->void {
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			studyAnim->JumpToAttackMontageSection(CurrentCombo);
		}
		studyAnim->OnAttackHitCheck.AddUObject(this, &AstudyCharacter::AttackCheck);
	});
	if (nullptr != CharacterStat)
	{
		CharacterStat->OnHPIsZero.AddLambda([this]()->void {
			ABLOG(Warning, TEXT("On HP is Zero"));
			studyAnim->SetDeadAnim();
			SetActorEnableCollision(false);
		});
	}

	auto CharacterWidget = Cast<UstudyCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
		CharacterWidget->BindCharacterStat(CharacterStat);

}

void AstudyCharacter::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	IsAttacking = false;
	AttackEndComboState();
	OnAttackEnd.Broadcast();
}

void AstudyCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1);
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AstudyCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AstudyCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(CharacterStat->GetAttack() , DamageEvent, GetController(), this);
	}
}

float AstudyCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);
	return FinalDamage;
}

void AstudyCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);
	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::DIABLO);
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}