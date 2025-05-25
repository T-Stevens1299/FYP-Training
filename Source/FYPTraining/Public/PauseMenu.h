// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"


UCLASS()
class FYPTRAINING_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UPauseMenu(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void toggleVisibility(bool isVisible);

	class AFYPTrainingGameMode* gmRef;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BackToGame;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuitGame;

private:
	UFUNCTION()
	void unPauseGame();

	UFUNCTION()
	void returnToMenu();
};