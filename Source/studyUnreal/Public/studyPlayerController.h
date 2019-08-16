// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "studyUnreal.h"
#include "GameFramework/PlayerController.h"
#include "studyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STUDYUNREAL_API AstudyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AstudyPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void Possess(APawn* aPawn) override;

	class UstudyHUDWidget* GetHUDWidget() const;
	void NPCKill(class AstudyCharacter* KilledNPC) const;
	void AddGameScore() const;
	void ChangeInputMode(bool bGameMode = true);
	void ShowResultUI();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI) TSubclassOf<class UstudyHUDWidget> HUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI) TSubclassOf<class UstudyGameplayWidget> MenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI) TSubclassOf<class UstudyGameplayResultWidget> ResultWidgetClass;

private:
	UPROPERTY() class UstudyHUDWidget* HUDWidget;
	UPROPERTY() class AstudyPlayerState* studyPlayerState;
	UPROPERTY() class UstudyGameplayResultWidget* ResultWidget;
	void OnGamePause();

	UPROPERTY() class UstudyGameplayWidget* MenuWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
};
