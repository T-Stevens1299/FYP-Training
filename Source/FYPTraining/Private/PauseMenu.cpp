// Written by Thomas Stevens, all rights reserved


#include "PauseMenu.h"
#include "Components/Button.h"
#include "FYPTraining/FYPTrainingGameMode.h"

UPauseMenu::UPauseMenu(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UPauseMenu::NativeConstruct()
{
	if (BackToGame)
	{
		BackToGame->OnClicked.AddDynamic(this, &UPauseMenu::unPauseGame);
	}

	if (QuitGame)
	{
		QuitGame->OnClicked.AddDynamic(this, &UPauseMenu::returnToMenu);
	}
}

void UPauseMenu::toggleVisibility(bool isVisible)
{
	if (isVisible) { this->SetVisibility(ESlateVisibility::Visible); }
	else { this->SetVisibility(ESlateVisibility::Hidden); }
}

void UPauseMenu::unPauseGame()
{
	UE_LOG(LogTemp, Warning, TEXT("UnPause"))
	gmRef->togglePauseGame(false);
	toggleVisibility(false);
}

void UPauseMenu::returnToMenu()
{
	//Quits the game early and pops up the results panel
	UE_LOG(LogTemp, Warning, TEXT("EndGame"))
	gmRef->gameEnd(true);
}