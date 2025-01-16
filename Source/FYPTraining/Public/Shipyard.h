// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectableObject.h"
#include "Shipyard.generated.h"

class AFYPTrainingGameMode;
class UShipyardWidget;

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

	void constructShip(TSubclassOf<AActor> shipToSpawn, float shipCost, float buildTime);

	void buildShip();

	void buildMines();

	void canUpgradeTechLevel(float upgradeCost, float upgradeTime);

	void upgradeLevel();

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	float IncomeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShipyardTweakables")
	float mineCost;

	float currentShipCost;

	bool isConstructingAlready;

	bool isUpgradingAlready;

	TSubclassOf<AActor> shipConstructing;

	//References
	AFYPTrainingGameMode* gmRef;
	
	FTimerHandle incomeTimer;

	FTimerHandle constructionTime;

	FTimerHandle upgradingTimer;

	APlayerController* PC;

	UPROPERTY(EditDefaultsOnly, Category = "ShipyardTweakables")
	TSubclassOf<UUserWidget> HUDref;

	UPROPERTY(BlueprintReadOnly, Category = "ShipyardTweakables")
	class UShipyardWidget* HUD;
};
