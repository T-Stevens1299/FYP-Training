// Written by Thomas Stevens, all rights reserved


#include "ShipyardWidget.h"
#include "Components/Button.h"
#include "Shipyard.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include "UnitManager.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UShipyardWidget::UShipyardWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UShipyardWidget::NativeConstruct()
{
	if (BuildCorvette)
	{
		BuildCorvette->OnClicked.AddDynamic(this, &UShipyardWidget::queueCorvette);
	}

	if (BuildFrigate)
	{
		BuildFrigate->OnClicked.AddDynamic(this, &UShipyardWidget::queueFrigate);
	}

	if (BuildDestroyer)
	{
		BuildDestroyer->OnClicked.AddDynamic(this, &UShipyardWidget::queueDestroyer);
	}

	if (BuildCruiser)
	{
		BuildCruiser->OnClicked.AddDynamic(this, &UShipyardWidget::queueCruiser);
	}

	if (BuildBattleship)
	{
		BuildBattleship->OnClicked.AddDynamic(this, &UShipyardWidget::queueBattleship);
	}

	if (TechLevel2)
	{
		TechLevel2->OnClicked.AddDynamic(this, &UShipyardWidget::queueTechLevel2);
	}

	if (TechLevel3)
	{
		TechLevel3->OnClicked.AddDynamic(this, &UShipyardWidget::queueTechLevel3);
	}

	if (TechLevel4)
	{
		TechLevel4->OnClicked.AddDynamic(this, &UShipyardWidget::queueTechLevel4);
	}

	if (BuildMines)
	{
		BuildMines->OnClicked.AddDynamic(this, &UShipyardWidget::triggerMineBuild);
	}
}

void UShipyardWidget::init(AShipyard* shipyardPtr)
{
	shipyardRef = shipyardPtr;
	currentTechLevel = 0;
	upgradeTechLevel();
}

void UShipyardWidget::triggerMineBuild()
{
	UE_LOG(LogTemp, Warning, TEXT("TriggerMineBuild"));
	shipyardRef->buildMines();
}

void UShipyardWidget::updateMineCount(bool addingMine)
{
	if (addingMine) { currentMineToBuild++; }
	else { currentMineToBuild--;  }

	MinesToBuild->SetText(FText::FromString(FString::FromInt(currentMineToBuild)));
}

void UShipyardWidget::updatePopCount(int popCap)
{
	PopCapText->SetText(FText::FromString(FString::FromInt(popCap)));
}


void UShipyardWidget::upgradeTechLevel()
{
	currentTechLevel++;

	//Sets tech level on player and AI side
	shipyardRef->gmRef->playerTechLevel = currentTechLevel;
	if (aiCanUpgrade) 
	{ 
		shipyardRef->managerRef->upgradeTechLevel(currentRow); 	
	}

	shipyardRef->gmRef->techLevelChanged(currentTechLevel);

	switch (currentTechLevel)
	{
	case 1:
		//Buttons to Show
		BuildCorvette->SetVisibility(ESlateVisibility::Visible);
		BuildCorvette->SetIsEnabled(true);

		BuildMines->SetVisibility(ESlateVisibility::Visible);
		BuildMines->SetIsEnabled(true);

		TechLevel2->SetVisibility(ESlateVisibility::Visible);
		TechLevel2->SetIsEnabled(true);

		break;
	case 2:
		//Buttons to Show
		BuildFrigate->SetVisibility(ESlateVisibility::Visible);
		BuildFrigate->SetIsEnabled(true);

		BuildDestroyer->SetVisibility(ESlateVisibility::Visible);
		BuildDestroyer->SetIsEnabled(true);

		TechLevel3->SetVisibility(ESlateVisibility::Visible);
		TechLevel3->SetIsEnabled(true);

		//Buttons to hide
		TechLevel2->SetVisibility(ESlateVisibility::Hidden);
		TechLevel2->SetIsEnabled(false);

		break;
	case 3:
		//Buttons to show
		BuildCruiser->SetVisibility(ESlateVisibility::Visible);
		BuildCruiser->SetIsEnabled(true);

		TechLevel4->SetVisibility(ESlateVisibility::Visible);
		TechLevel4->SetIsEnabled(true);

		//Buttons to hide
		TechLevel3->SetVisibility(ESlateVisibility::Hidden);
		TechLevel3->SetIsEnabled(false);

		break;
	case 4:
		//Buttons to show
		BuildBattleship->SetVisibility(ESlateVisibility::Visible);
		BuildBattleship->SetIsEnabled(true);

		//Buttons to hide
		TechLevel4->SetVisibility(ESlateVisibility::Hidden);
		TechLevel4->SetIsEnabled(false);
		break;
	default:
		break;
	}
}

void UShipyardWidget::triggerTechLevelCheck(FString techLevelRowName)
{
	FName rowToFind = FName(*techLevelRowName);
	currentRow = dataTableRef.DataTable->FindRow<FConstructionData>(rowToFind, "");
	//Triggers the can upgrade tech function. If true is returned the upgrade process starts and boolean variable set for later to upgrade the AI
	aiCanUpgrade = shipyardRef->canUpgradeTechLevel(currentRow->requiredFunds, currentRow->constructionTime);
}

void UShipyardWidget::updateConstructionBar(float passedPercentage)
{
	ConstructionProgressBar->SetPercent(passedPercentage);
}

void UShipyardWidget::updateTechBar(float passedPercentage)
{
	TechUpgradeBar->SetPercent(passedPercentage);
}

void UShipyardWidget::queueCorvette()
{
	addShipToQueue("Corvette");
}

void UShipyardWidget::queueFrigate()
{
	addShipToQueue("Frigate");
}

void UShipyardWidget::queueDestroyer()
{
	addShipToQueue("Destroyer");
}

void UShipyardWidget::queueCruiser()
{
	addShipToQueue("Cruiser");
}

void UShipyardWidget::queueBattleship()
{
	addShipToQueue("Battleship");
}

void UShipyardWidget::queueAircraftCarrier()
{
	addShipToQueue("AircraftCarrier");
}

void UShipyardWidget::queueTechLevel2()
{
	triggerTechLevelCheck("TechLevel2");
}

void UShipyardWidget::queueTechLevel3()
{
	triggerTechLevelCheck("TechLevel3");
}

void UShipyardWidget::queueTechLevel4()
{
	triggerTechLevelCheck("TechLevel4");
}

void UShipyardWidget::addShipToQueue(FString RelatedRowName)
{
	//Finds the ship to queue based on data table string
	FName rowToFind = FName(*RelatedRowName);
	currentRow = dataTableRef.DataTable->FindRow<FConstructionData>(rowToFind, "");

	//Checks the ship can be queued, if so add it to the queue
	if (shipyardRef->canQueueShip(currentRow->requiredFunds, currentRow->constructionTime, currentRow->populationValue))
	{
		shipQueue.Add(RelatedRowName);
	}

	//If the queue is empty - build the queued ship immediately
	if (shipQueue.Num() < 2)
	{
		shipyardRef->constructCurrentQueuedShip(currentRow->requiredFunds, currentRow->constructionTime, currentRow->populationValue);
	}
}

void UShipyardWidget::buildNextShipInQueue()
{
	//Removes just built ship from the queue
	shipQueue.RemoveAt(0, 1, true);

	//If the queue is now empty do nothing
	if (shipQueue.Num() < 1) { UE_LOG(LogTemp, Warning, TEXT("QueueEmpty")); return; }
	else
	{
		//Build the ship at the front of the queue
		FName rowToFind = FName(*shipQueue[0]);
		currentRow = dataTableRef.DataTable->FindRow<FConstructionData>(rowToFind, "");
		shipyardRef->constructCurrentQueuedShip(currentRow->requiredFunds, currentRow->constructionTime, currentRow->populationValue);
	}
}