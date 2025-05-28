// Written by Thomas Stevens, all rights reserved


#include "ShipyardWidget.h"
#include "Components/Button.h"
#include "Shipyard.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include "UnitManager.h"
#include "Components/ProgressBar.h"
#include "Components/RadialSlider.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

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

	if (TechButton)
	{
		TechButton->OnClicked.AddDynamic(this, &UShipyardWidget::stopTechUpgrade);
	}

	if (QueueButton1)
	{
		QueueButton1->OnClicked.AddDynamic(this, &UShipyardWidget::stopCurrentShipConstruction);
	}

	if (QueueButton2)
	{
		QueueButton2->OnClicked.AddDynamic(this, &UShipyardWidget::clearQueueSlot1);
	}

	if (QueueButton3)
	{
		QueueButton3->OnClicked.AddDynamic(this, &UShipyardWidget::clearQueueSlot2);
	}

	if (QueueButton4)
	{
		QueueButton4->OnClicked.AddDynamic(this, &UShipyardWidget::clearQueueSlot3);
	}

	if (QueueButton5)
	{
		QueueButton5->OnClicked.AddDynamic(this, &UShipyardWidget::clearQueueSlot4);
	}
}

void UShipyardWidget::init(AShipyard* shipyardPtr)
{
	shipyardRef = shipyardPtr;
	currentTechLevel = 0;
	upgradeTechLevel();

	queueButtons.Add(QueueButton1);
	queueButtons.Add(QueueButton2);
	queueButtons.Add(QueueButton3);
	queueButtons.Add(QueueButton4);
	queueButtons.Add(QueueButton5);
}

void UShipyardWidget::triggerMineBuild()
{
	UE_LOG(LogTemp, Warning, TEXT("TriggerMineBuild"));
	shipyardRef->buildMines();
}

void UShipyardWidget::stopTechUpgrade()
{
	FConstructionData* techLevelToCancel;

	//Determines the current tech upgrade to cancel based on the current tech level
	switch (currentTechLevel)
	{
	case 1:
		techLevelToCancel = getRow("TechLevel2");
		break;
	case 2:
		techLevelToCancel = getRow("TechLevel3");
		break;
	case 3:
		techLevelToCancel = getRow("TechLevel4");
		break;
	default:
		techLevelToCancel = getRow("TechLevel2");
		break;
	}

	//Stops the tech level, hides the button and resets progress value
	TechUpgradeCircle->SetValue(0.f);
	shipyardRef->stopTechLevelUpgrade(techLevelToCancel->requiredFunds); 
	TechButton->SetVisibility(ESlateVisibility::Hidden);
}

void UShipyardWidget::stopCurrentShipConstruction()
{
	FConstructionData* rowToCancel = getRow(*shipQueue[0]);
	shipyardRef->stopCurShipConstruction(rowToCancel->requiredFunds, rowToCancel->populationValue);
}

void UShipyardWidget::clearQueueSlot1()
{
	clearQueuedItem(QueueButton2, 1);
}

void UShipyardWidget::clearQueueSlot2()
{
	clearQueuedItem(QueueButton3, 2);
}

void UShipyardWidget::clearQueueSlot3()
{
	clearQueuedItem(QueueButton4, 3);
}

void UShipyardWidget::clearQueueSlot4()
{
	clearQueuedItem(QueueButton5, 4);
}

void UShipyardWidget::updateMineCount(bool addingMine)
{
	if (addingMine) { currentMineToBuild++; }
	else { currentMineToBuild--;  }

	MinesToBuild->SetText(FText::FromString(FString::FromInt(currentMineToBuild)));
}

void UShipyardWidget::toggleConstructionOptions(bool showUI)
{
	if (showUI) { ConstructionUI->SetVisibility(ESlateVisibility::Visible); }
	else { ConstructionUI->SetVisibility(ESlateVisibility::Hidden); }
}


void UShipyardWidget::upgradeTechLevel()
{
	currentTechLevel++;

	//Sets tech level on player and AI side
	shipyardRef->gmRef->playerTechLevel = currentTechLevel;
	TechButton->SetVisibility(ESlateVisibility::Hidden);

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
	currentRow = getRow(techLevelRowName);
	//Triggers the can upgrade tech function. If true is returned the upgrade process starts and boolean variable set for later to upgrade the AI
	aiCanUpgrade = shipyardRef->canUpgradeTechLevel(currentRow->requiredFunds, currentRow->constructionTime);

	TechButton->SetVisibility(ESlateVisibility::Visible);
	TechButton->WidgetStyle.Normal.SetResourceObject(techLevelIcons[techLevelRowName]);
	TechButton->WidgetStyle.Hovered.SetResourceObject(techLevelIcons[techLevelRowName]);
	TechButton->WidgetStyle.Pressed.SetResourceObject(techLevelIcons[techLevelRowName]);
}

void UShipyardWidget::clearQueuedItem(UButton* buttonRef, int queueIndex)
{
	//Hides cancelled button
	buttonRef->SetVisibility(ESlateVisibility::Hidden);

	//Gets the cost and pop value of the current queued ship and refunds it
	FConstructionData* rowToRefund = getRow(shipQueue[queueIndex]);
	shipyardRef->refundQueuedShip(rowToRefund->requiredFunds, rowToRefund->populationValue);
	
	//Removes it from the queue and re-adjusts the queue order
	shipQueue.RemoveAt(queueIndex, 1, true);
	alterQueueOrder();
}

void UShipyardWidget::setButtonIcon(UButton* buttonRef, FString RowKey)
{
	buttonRef->SetVisibility(ESlateVisibility::Visible);
	buttonRef->WidgetStyle.Normal.SetResourceObject(shipIcons[RowKey]);
	buttonRef->WidgetStyle.Hovered.SetResourceObject(shipIcons[RowKey]);
	buttonRef->WidgetStyle.Pressed.SetResourceObject(shipIcons[RowKey]);
}

void UShipyardWidget::alterQueueOrder()
{
	int buttonsToHide = 0;

	//Resets the icons of all the buttons after an element from the queue was removed
	for (int i = 0; i < shipQueue.Num(); i++)
	{
		buttonsToHide++;
		setButtonIcon(queueButtons[i], shipQueue[i]);
	}

	//Hides buttons that should not be showing queued items
	for (int i = buttonsToHide; i < 5; i++)
	{
		queueButtons[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

FConstructionData* UShipyardWidget::getRow(FString RelatedRowName)
{
	FName rowToFind = FName(*RelatedRowName);
	return dataTableRef.DataTable->FindRow<FConstructionData>(rowToFind, "");
}

void UShipyardWidget::updateConstructionBar(float passedPercentage)
{
	ShipConstructionCircle->SetValue(passedPercentage);
}

void UShipyardWidget::updateTechBar(float passedPercentage)
{
	TechUpgradeCircle->SetValue(passedPercentage);
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
	currentRow = getRow(RelatedRowName);

	//If the queue is full nothing can be queued
	if (shipQueue.Num() >= 5) { return; }

	//Checks the ship can be queued, if so add it to the queue
	if (shipyardRef->canQueueShip(currentRow->requiredFunds, currentRow->constructionTime, currentRow->populationValue))
	{
		int indexInQueue = shipQueue.Add(RelatedRowName);
		switch (indexInQueue)
		{
		case 0:
			setButtonIcon(QueueButton1, RelatedRowName);
			break;
		case 1:
			setButtonIcon(QueueButton2, RelatedRowName);
			break;
		case 2:
			setButtonIcon(QueueButton3, RelatedRowName);
			break;
		case 3:
			setButtonIcon(QueueButton4, RelatedRowName);
			break;
		case 4:
			setButtonIcon(QueueButton5, RelatedRowName);
			break;

		default:
			break;
		}
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

	//Re-orders the queue buttons
	alterQueueOrder();

	//Resets progress bar
	ShipConstructionCircle->SetValue(0.f);

	//If the queue is now empty do nothing
	if (shipQueue.Num() < 1) { QueueButton1->SetVisibility(ESlateVisibility::Hidden); return; }
	else
	{
		//Build the ship at the front of the queue
		currentRow = getRow(*shipQueue[0]);
		shipyardRef->constructCurrentQueuedShip(currentRow->requiredFunds, currentRow->constructionTime, currentRow->populationValue);
	}
}