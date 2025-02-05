// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectableObject.h"
#include "Shipyard.generated.h"

class AFYPTrainingGameMode;
class UShipyardWidget;
class AUnitManager;

UCLASS()
class FYPTRAINING_API AShipyard : public ASelectableObject
{
	GENERATED_BODY()

public:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreParts", meta = (AllowPrivateAccess = "true"))
	USceneComponent* unitSpawner;

	//Functions
	AShipyard();

	void init(AFYPTrainingGameMode* gamemodeReference);

	bool constructShip(TSubclassOf<AActor> shipToSpawn, float shipCost, float buildTime, int popValue);

	void buildMines();

	bool canUpgradeTechLevel(float upgradeCost, float upgradeTime);


	void spawnStartingShips();

	AActor* spawnShip(TSubclassOf<AActor> shipToSpawn);

	virtual void triggerWinCheck() override;

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	float IncomeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	float mineCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	int startingShipCount;

	float currentShipCost;

	int currentShipPopValue;

	bool isConstructingAlready;

	bool isUpgradingAlready;

	TSubclassOf<AActor> shipConstructing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	TSubclassOf<AActor> startingShipRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	AActor* retreatPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	AActor* attackPoint;

	//References
	AFYPTrainingGameMode* gmRef;
	
	FTimerHandle healthBarDelay;

	FTimerHandle constructionTime;

	FTimerHandle upgradingTimer;

	FTimerHandle conProgressTimer;

	APlayerController* PC;

	AUnitManager* managerRef;

	UPROPERTY(EditDefaultsOnly, Category = "ShipyardTweakables")
	TSubclassOf<UUserWidget> HUDref;

	UPROPERTY(BlueprintReadOnly, Category = "ShipyardTweakables")
	class UShipyardWidget* HUD;

private:
	void buildShip();

	void buildShipProgress();

	void healthBarSet();

	void upgradeLevelProgress();

	void upgradeLevel();

	float curShipConTime = 0.0f;

	float constructionProgress = 0.0f;

	float curTechUpgradeTime = 0.0f;

	float techUpgradeProgress = 0.0f;
};
