// Written by Thomas Stevens, all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Blueprint/UserWidget.h"
#include "ShipyardWidget.generated.h"

class AShipyard;
class ASelectableObject;


USTRUCT(BlueprintType)
struct FConstructionData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AssetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float requiredFunds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float constructionTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int populationValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int buildWeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AActor> shipToSpawn;
};


UCLASS()
class FYPTRAINING_API UShipyardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//Functions
	UShipyardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void addShipToQueue(FString RelatedRowName);

	void init(AShipyard* shipyardPtr);

	void upgradeTechLevel();

	void updateConstructionBar(float passedPercentage);

	void updateTechBar(float passedPercentage);

	void updateMineCount(bool addingMine);

	void updatePopCount(int popCap);

	//References
	AShipyard* shipyardRef;

	FConstructionData* currentRow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle dataTableRef;

private:
	//Variables
	int currentTechLevel;

	bool aiCanUpgrade;

	int currentMineToBuild = 0;

	//Functions
	void triggerTechLevelCheck(FString techLevelRowName);

protected:
	//UI Components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildCorvette;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildFrigate;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildDestroyer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildCruiser;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildBattleship;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildAircraftCarrier;



	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* TechLevel2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* TechLevel3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* TechLevel4;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* HealthUpgrade1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* HealthUpgrade2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* HealthUpgrade3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* HealthUpgrade4;



	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* DamageUpgrade1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* DamageUpgrade2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* DamageUpgrade3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* DamageUpgrade4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildMines;



	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ConstructionProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* TechUpgradeBar;



	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MinesToBuild;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* PopCapText;

	//Bound Functions
	UFUNCTION()
	void queueCorvette();

	UFUNCTION()
	void queueFrigate();

	UFUNCTION()
	void queueDestroyer();

	UFUNCTION()
	void queueCruiser();

	UFUNCTION()
	void queueBattleship();

	UFUNCTION()
	void queueAircraftCarrier();

	UFUNCTION()
	void queueTechLevel2();

	UFUNCTION()
	void queueTechLevel3();

	UFUNCTION()
	void queueTechLevel4();

	UFUNCTION()
	void triggerMineBuild();
};
