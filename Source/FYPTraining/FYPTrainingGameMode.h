// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FYPTrainingGameMode.generated.h"

class UUserWidget;
class UPlayerHUD;

UCLASS(minimalapi)
class AFYPTrainingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFYPTrainingGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Variables
	float currentPlayerMoney;

	float currentAIMoney;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	float aiIncomeMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	float startingAiFunds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	float startingPlayerFunds;

	/** HUD Widget */
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> HUDref;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	UPlayerHUD* HUD;

	APlayerController* PC;

	//Functions
	void IncreaseIncome(bool isAiControlled, float moneyToAdd);

};