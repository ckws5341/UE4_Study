// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "studyUnreal.h"
#include "GameFramework/Actor.h"
#include "studyItemBox.generated.h"

UCLASS()
class STUDYUNREAL_API AstudyItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AstudyItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:
	UPROPERTY(VisibleAnywhere, Category = Box) UBoxComponent* Trigger;
	UPROPERTY(VisibleAnywhere, Category = Box) UStaticMeshComponent* Box;
	UPROPERTY(VisibleAnywhere, Category = Effect) UParticleSystemComponent* Effect;
	UPROPERTY(EditInstanceOnly, Category = Box) TSubclassOf<class AstudyWeapon> WeaponItemClass;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private :
	UFUNCTION() void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFrmpSweep, const FHitResult& SweepResult);
	
	UFUNCTION() void OnEffectFinished(class UParticleSystemComponent* Psystem);

	
};
