// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AdminPanel.generated.h"

class AFYPTrainingGameMode;

/**
 * 
 */
UCLASS()
class FYPTRAINING_API UAdminPanel : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UAdminPanel(const FObjectInitializer& ObjectInitializer);

	void init(AFYPTrainingGameMode* gamemodeRef);

	void toggleVisibility();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Funds;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* IPS;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Difficulty;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* IM;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* PopCount;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TechLevel;

private:
	void grabAiStats();

	void setAiStats();

	bool isHidden = false;

	float aiFunds;
	float aiIPS;
	float aiDifficulty;
	float aiIncomeMultiplier;
	int aiPopCap;
	int aiTechLevel;

	AFYPTrainingGameMode* gmRef;

	FTimerHandle updateStatTimer;
};
