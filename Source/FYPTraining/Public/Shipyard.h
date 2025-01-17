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

	void generateIncome(float prevIncomeRate, bool techUpgrade);

	void constructShip(TSubclassOf<AActor> shipToSpawn, float shipCost, float buildTime, int popValue);

	void buildShip();

	void buildMines();

	bool canUpgradeTechLevel(float upgradeCost, float upgradeTime);

	void upgradeLevel();

	virtual void triggerWinCheck() override;

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	float IncomeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	float mineCost;

	float currentShipCost;

	int currentShipPopValue;

	bool isConstructingAlready;

	bool isUpgradingAlready;

	TSubclassOf<AActor> shipConstructing;

	//References
	AFYPTrainingGameMode* gmRef;
	
	FTimerHandle incomeTimer;

	FTimerHandle constructionTime;

	FTimerHandle upgradingTimer;

	APlayerController* PC;

	AUnitManager* managerRef;

	UPROPERTY(EditDefaultsOnly, Category = "ShipyardTweakables")
	TSubclassOf<UUserWidget> HUDref;

	UPROPERTY(BlueprintReadOnly, Category = "ShipyardTweakables")
	class UShipyardWidget* HUD;
};
