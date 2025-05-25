// Written by Thomas Stevens, all rights reserved


#include "PlayerHUD.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "FYPTraining/FYPTrainingGameMode.h"

UPlayerHUD::UPlayerHUD(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UPlayerHUD::NativeConstruct()
{
	if (surrender)
	{
		surrender->OnClicked.AddDynamic(this, &UPlayerHUD::surrenderTheGame);
	}
}

void UPlayerHUD::SetGmPtr(AFYPTrainingGameMode* GMptr)
{
	GMref = GMptr;
	surrender->SetIsEnabled(false);
}

void UPlayerHUD::updatePopCap(int curPopCapValue)
{
	PopCap->SetText(FText::FromString(FString::FromInt(curPopCapValue)));
}

//Updates the player funds
void UPlayerHUD::updateFunds(float passedFunds)
{
	PlayerFunds->SetText(FText::FromString(FString::SanitizeFloat(passedFunds)));
}

//Enables the surrender button
void UPlayerHUD::unlockSurrender()
{
	surrender->SetIsEnabled(true);
}

//Allows the player to surrender which ends the game immediately
void UPlayerHUD::surrenderTheGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Pressed"))
	GMref->gameEnd(false);
}