// Fill out your copyright notice in the Description page of Project Settings.

#include "studyCharacter.h"
#include "studyAnimInstance.h"
#include "studyWeapon.h"
#include "StudyCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "studyCharacterWidget.h"
#include "studyAIController.h"
#include "studyCharacterSetting.h"
#include "StudyGameInstance.h"
#include "studyPlayerController.h"
#include "studyPlayerState.h"
#include "studyHUDWidget.h"
#include "studyGameMode.h"


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

	AttackRange = 80.f;
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

	auto DefaultSetting = GetDefault<UstudyCharacterSetting>();
	if (DefaultSetting->CharacterAssets.Num() > 0)
	{
		for (auto CharacterAsset : DefaultSetting->CharacterAssets)
		{
			ABLOG(Warning, TEXT("Character Asset : %s"), *CharacterAsset.ToString());
		}
	}

	AssetIndex = 4;
	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	bCanBeDamaged = false;

	DeadTimer = 5.f;
}

// Called when the game starts or when spawned
void AstudyCharacter::BeginPlay()
{
	Super::BeginPlay();
	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		studyPlayerController = Cast<AstudyPlayerController>(GetController());
	}
	else
	{
		studyAIController = Cast<AstudyAIController>(GetController());
	}

	auto DefaultSetting = GetDefault<UstudyCharacterSetting>();
	if (bIsPlayer)
	{
		auto studyPlayerState = Cast<AstudyPlayerState>(PlayerState);
		if (nullptr != studyPlayerState)
			AssetIndex = studyPlayerState->GetCharacterIndex();
	}
	else
	{
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}

	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	auto studyGameInstance = Cast<UStudyGameInstance>(GetGameInstance());
	if (nullptr != studyGameInstance)
	{
		AssetStreamingHandle = studyGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AstudyCharacter::OnAssetLoadCompleted));
		SetCharacterState(ECharacterState::LOADING);
	}
}
void AstudyCharacter::SetCharacterState(ECharacterState NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
		switch (CurrentState)
		{
		case ECharacterState::LOADING:
		{
			if (bIsPlayer)
			{
				DisableInput(studyPlayerController);

				studyPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);


				auto studyPlayerState = Cast<AstudyPlayerState>(PlayerState);
				if (nullptr != studyPlayerState)
					CharacterStat->SetNewLevel(studyPlayerState->GetCharacterLevel());
			}
			else
			{
				auto studyGameMode = Cast<AstudyGameMode>(GetWorld()->GetAuthGameMode());
				if (nullptr != studyGameMode)
				{
					int32 TargetLevel = FMath::CeilToInt(((float)studyGameMode->GetScore() * 0.8f));
					int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
					ABLOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
					CharacterStat->SetNewLevel(FinalLevel);
				}
			}
			SetActorHiddenInGame(true);
			HPBarWidget->SetHiddenInGame(true);
			bCanBeDamaged = false;
			break;
			
		}
		case ECharacterState::READY:
		{
			SetActorHiddenInGame(false);
			HPBarWidget->SetHiddenInGame(false);
			bCanBeDamaged = true;
			CharacterStat->OnHPIsZero.AddLambda([this]()->void {
				SetCharacterState(ECharacterState::DEAD);
			});

			auto CharacterWidget = Cast<UstudyCharacterWidget>(HPBarWidget->GetUserWidgetObject());
			if (nullptr != CharacterWidget)
			{
				CharacterWidget->BindCharacterStat(CharacterStat);
			}
			if (bIsPlayer)
			{
				SetControlMode(EControlMode::DIABLO);
				GetCharacterMovement()->MaxWalkSpeed = 600.f;
				EnableInput(studyPlayerController);
			}
			else
			{
				SetControlMode(EControlMode::NPC);
				GetCharacterMovement()->MaxWalkSpeed = 300.f;
				studyAIController->RunAI();
			}
			break;
		}
		case ECharacterState::DEAD:
		{
			SetActorEnableCollision(false);
			GetMesh()->SetHiddenInGame(false);
			HPBarWidget->SetHiddenInGame(true);
			studyAnim->SetDeadAnim();
			bCanBeDamaged = false;
			if (bIsPlayer)
			{
				studyPlayerController->ShowResultUI();
			}
			else
				studyAIController->StopAI();

			GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
				if (bIsPlayer)
					studyPlayerController->RestartLevel();
				else
					Destroy();
			}), DeadTimer, false);
			break;
		}
		}
	}
}
ECharacterState AstudyCharacter::GetCharacterState() const
{
	return CurrentState;
}

int32 AstudyCharacter::GetExp() const
{
	return CharacterStat->GetDropExp();
}
float AstudyCharacter::GetFinalAttackRange() const
{
	return(nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackRange() : AttackRange;
}
float AstudyCharacter::GetFinalAttackDamage() const
{
	float AttackDamage = (nullptr != CurrentWeapon) ? (CharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage()) : CharacterStat->GetAttack();
	float AttackModifier = (nullptr != CurrentWeapon) ? (CurrentWeapon->GetAttackModifier()) : 1.f;
	return AttackDamage * AttackModifier;
}

bool AstudyCharacter::CanSetWeapon()
{
	return true;
}

void AstudyCharacter::SetWeapon(AstudyWeapon* NewWeapon)
{
	if (nullptr != NewWeapon)
	{
		if (nullptr != CurrentWeapon)
		{
			CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurrentWeapon->Destroy();
			CurrentWeapon = nullptr;
		}
	}
	FName WeaponSocket(TEXT("hand_rSocket"));
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
		
	});
	studyAnim->OnAttackHitCheck.AddUObject(this, &AstudyCharacter::AttackCheck);
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
	float FinalAttackRange = GetFinalAttackRange();
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * FinalAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = FinalAttackRange * 0.5f + AttackRadius;
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
		{
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}
}

float AstudyCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);
	if (CurrentState == ECharacterState::DEAD)
	{
		if (EventInstigator->IsPlayerController())
		{
			auto studyPlayerController = Cast<AstudyPlayerController>(EventInstigator);
			if (nullptr != studyPlayerController)
				studyPlayerController->NPCKill(this);
			else return 0.f;
		}
	}
	return FinalDamage;
}

void AstudyCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);
}

void AstudyCharacter::OnAssetLoadCompleted()
{
	AssetStreamingHandle->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(CharacterAssetToLoad);
	if (LoadedAssetPath.IsValid())
	{
		GetMesh()->SetSkeletalMesh(LoadedAssetPath.Get());
		SetCharacterState(ECharacterState::READY);
	}
}
