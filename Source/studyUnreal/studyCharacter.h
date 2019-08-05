// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "studyUnreal.h"
#include "GameFramework/Character.h"
#include "studyCharacter.generated.h"

UCLASS()
class STUDYUNREAL_API AstudyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AstudyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		GTA, DIABLO
	};

	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;

	FVector DirectionToMove = FVector::ZeroVector;

	float ArmLengthTo = 0.f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.f;
	float ArmRotationSpeed = 0.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	bool CanSetWeapon();
	void SetWeapon(class AstudyWeapon* NewWeapon);

	UPROPERTY(VisibleAnywhere, Category = Weapon) class AstudyWeapon* CurrentWeapon;
	UPROPERTY(VisibleAnywhere, Category = Camera) USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera) UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = Stat) class UStudyCharacterStatComponent* CharacterStat;
	UPROPERTY(VisibleAnywhere, Category = UI) class UWidgetComponent* HPBarWidget;

private :
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float newAxisValue);

	void ViewChange();
	void Attack();
	
	UFUNCTION() void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();

private : 
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) int32 MaxCombo;

	UPROPERTY() class UstudyAnimInstance* studyAnim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true)) float AttackRadius;



};
