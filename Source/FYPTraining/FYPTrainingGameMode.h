// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FYPTrainingGameMode.generated.h"

class UUserWidget;
class UPlayerHUD;
class UGameEndScreen;
class AShipyard;
class UAdminPanel;
class AAIMasterControlManager;

//class EFactionTag;

UCLASS(minimalapi)
class AFYPTrainingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	//Constructor
	AFYPTrainingGameMode();

	//Variables
	UPROPERTY(EditAnywhere, Category = "GameStats")
	float currentPlayerMoney;

	UPROPERTY(EditAnywhere, Category = "GameStats")
	float currentAIMoney;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	bool useDDS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	bool useLanchester;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	float aiIncomeMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	float currentaiDifficulty = 0.4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	float startingAiFunds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	float startingPlayerFunds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStats")
	float aiIncomePerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStats")
	float playerIncomePerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStats")
	int playerPopCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStats")
	int currentPlayerPopCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStats")
	int aiPopCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStats")
	int currentAiPopCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStats")
	int playerTechLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStats")
	int aiTechLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStats")
	TArray<float> techLevelIncomeIncreases;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	TSubclassOf<AActor> aiManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITweakables")
	AAIMasterControlManager* aiManagerRef;

	/** HUD Widget */
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> HUDref;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	UPlayerHUD* HUD;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> gameEndRef;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	UGameEndScreen* GameEnd;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> adminPanelRef;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	UAdminPanel* adminPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceMines")
	TArray<AActor*> AIResourceMine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceMines")
	TArray<AActor*> PlayerResourceMine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipArrays")
	TArray<AActor*> ActiveAiShips;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipArrays")
	TArray<AActor*> ActivePlayerShips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipArrays")
	TArray<FString> playerShipsLost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipArrays")
	TArray<FString> aiShipsLost;

	APlayerController* PC;

	//Functions
	UFUNCTION(BlueprintCallable, Category = "Admin")
	void ToggleAdminPanel();

	void increaseIncomePerSecond(bool playerControlled, float incomeToIncrease);

	void subtractCost(bool playerControlled, float incomeToSubtract);

	void techLevelChanged(int passedTechLevel);

	void updatePopCap(bool playerControlled, int popValue);

	void updateMineStatus(AActor* passedMine, bool playerControlled, bool isCaptured);

	void addShipsToArray(AActor* shipToAdd, bool isPlayerControlled);

	void removeShipsFromArray(AActor* shipToAdd, FString className, bool isPlayerControlled);

	void gameEnd(bool playerControlled);

protected:
	virtual void BeginPlay() override;

private:
	int matchDuration = 0;

	float currentBaseIncomeRate;

	void IncreaseIncome();

	void setMines();

	void setShipyards();

	void calculateLostShips();

	void increaseIncomePerTechLevel(int passTechLevel);

	void saveData();

	void giveUpButton();

	FTimerHandle incomeHandle;
	
	FTimerHandle giveUpTimer;

	AShipyard* playerShipyardRef;

	TArray<FString> dataToSave;
};