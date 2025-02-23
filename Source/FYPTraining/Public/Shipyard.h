// Written by Thomas Stevens, all rights reserved

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

protected:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreParts", meta = (AllowPrivateAccess = "true"))
	USceneComponent* unitSpawner;

public:
	//Functions
	AShipyard();

	void init(AFYPTrainingGameMode* gamemodeReference);

	bool constructShip(TSubclassOf<AActor> shipToSpawn, float shipCost, float buildTime, int popValue);

	void buildMines();

	bool canUpgradeTechLevel(float upgradeCost, float upgradeTime);

	virtual void triggerWinCheck() override;

	//Variables
	float currentShipCost;

	int currentShipPopValue;

	bool isConstructingAlready;

	bool isUpgradingAlready;

	TSubclassOf<AActor> shipConstructing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	float IncomeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	float mineCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	int startingShipCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	TSubclassOf<AActor> startingShipRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	AActor* retreatPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	AActor* attackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipsToSpawn")
	TSubclassOf<AActor> corvetteRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipsToSpawn")
	TSubclassOf<AActor> frigateRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipsToSpawn")
	TSubclassOf<AActor> destroyerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipsToSpawn")
	TSubclassOf<AActor> cruiserRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipsToSpawn")
	TSubclassOf<AActor> battleshipRef;

	UPROPERTY(EditDefaultsOnly, Category = "ShipyardTweakables")
	TSubclassOf<UUserWidget> HUDref;

	UPROPERTY(BlueprintReadOnly, Category = "ShipyardTweakables")
	class UShipyardWidget* HUD;

	//References
	AFYPTrainingGameMode* gmRef;

	AUnitManager* managerRef;

private:
	//Variables
	float curShipConTime = 0.0f;

	float constructionProgress = 0.0f;

	float curTechUpgradeTime = 0.0f;

	float techUpgradeProgress = 0.0f;

	//References
	
	FTimerHandle healthBarDelay;

	FTimerHandle constructionTime;

	FTimerHandle upgradingTimer;

	FTimerHandle conProgressTimer;

	APlayerController* PC;

	//Functions
	void buildShip();

	void buildShipProgress();

	void healthBarSet();

	void upgradeLevelProgress();

	void upgradeLevel();

	void spawnStartingShips();

	AActor* spawnShip(TSubclassOf<AActor> shipToSpawn);
};