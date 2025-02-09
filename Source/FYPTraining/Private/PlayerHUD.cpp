// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens


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
}

void UPlayerHUD::updateFunds(float passedFunds)
{
	PlayerFunds->SetText(FText::FromString(FString::SanitizeFloat(passedFunds)));
}

void UPlayerHUD::surrenderTheGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Pressed"))
	GMref->gameEnd(false);
}