// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FYPTrainingGameMode.generated.h"

class UUserWidget;
class UPlayerHUD;
class AShipyard;
class AAIMasterControlManager;

//class EFactionTag;

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
	UPROPERTY(BlueprintReadOnly, Category = "AITweakables")
	float currentPlayerMoney;

	UPROPERTY(BlueprintReadOnly, Category = "AITweakables")
	float currentAIMoney;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	float aiIncomeMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	float startingAiFunds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	float startingPlayerFunds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	float aiIncomePerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	float playerIncomePerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	TSubclassOf<AActor> aiManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	AAIMasterControlManager* aiManagerRef;

	/** HUD Widget */
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> HUDref;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	UPlayerHUD* HUD;

	APlayerController* PC;

	FTimerHandle incomeHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceMines")
	TArray<AActor*> AIResourceMine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceMines")
	TArray<AActor*> PlayerResourceMine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipArrays")
	TArray<AActor*> ActiveAiShips;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipArrays")
	TArray<AActor*> ActivePlayerShips;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AITweakables")
	//EFactionTag FactionTag;

	//Functions
	void IncreaseIncome();

	void increaseIncomePerSecond(bool playerControlled, float incomeToIncrease);

	void subtractCost(bool playerControlled, float incomeToSubtract);

	void setShipyards();

	void setMines();

	void updateMineStatus(AActor* passedMine, bool playerControlled, bool isCaptured);

	void addShipsToArray(AActor* shipToAdd, bool isPlayerControlled);

	void removeShipsFromArray(AActor* shipToAdd, bool isPlayerControlled);

};