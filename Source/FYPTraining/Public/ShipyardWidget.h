// Written by Thomas Stevens, all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Blueprint/UserWidget.h"
#include "ShipyardWidget.generated.h"

class AShipyard;
class ASelectableObject;
class UButton;


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

	void buildNextShipInQueue();

	void alterQueueOrder();

	void init(AShipyard* shipyardPtr);

	void upgradeTechLevel();

	void updateConstructionBar(float passedPercentage);

	void updateTechBar(float passedPercentage);

	void updateMineCount(bool addingMine);

	void updatePopCount(int popCap);

	void toggleConstructionOptions(bool showUI);

	TArray<FString> shipQueue;

	//References
	AShipyard* shipyardRef;

	FConstructionData* currentRow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle dataTableRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FString, UObject*> shipIcons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FString, UObject*> techLevelIcons;

private:
	//Variables
	int currentTechLevel;

	int currentMineToBuild = 0;

	bool aiCanUpgrade;

	//Functions
	void triggerTechLevelCheck(FString techLevelRowName);

	void clearQueuedItem(UButton* buttonRef, int queueIndex);

	void setButtonIcon(UButton* buttonRef, FString RowKey);

	FConstructionData* getRow(FString RelatedRowName);

	TArray<UButton*> queueButtons;

protected:
	//UI Components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* ConstructionUI;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BuildCorvette;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BuildFrigate;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BuildDestroyer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BuildCruiser;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BuildBattleship;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BuildAircraftCarrier;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* TechLevel2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* TechLevel3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* TechLevel4;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* HealthUpgrade1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* HealthUpgrade2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* HealthUpgrade3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* HealthUpgrade4;



	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DamageUpgrade1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DamageUpgrade2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DamageUpgrade3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DamageUpgrade4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BuildMines;

	//Queue Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* TechButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* QueueButton1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* QueueButton2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* QueueButton3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* QueueButton4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* QueueButton5;


	//Progress Sliders
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class URadialSlider* ShipConstructionCircle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class URadialSlider* TechUpgradeCircle;



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

	UFUNCTION()
	void stopTechUpgrade();

	UFUNCTION()
	void stopCurrentShipConstruction();

	UFUNCTION()
	void clearQueueSlot1();

	UFUNCTION()
	void clearQueueSlot2();

	UFUNCTION()
	void clearQueueSlot3();

	UFUNCTION()
	void clearQueueSlot4();
};
