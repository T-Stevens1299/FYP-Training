// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens


#include "PlayerHUD.h"
#include "Components/TextBlock.h"

UPlayerHUD::UPlayerHUD(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UPlayerHUD::SetGmPtr(AFYPTrainingGameMode* GMptr)
{
	GMref = GMptr;
}

void UPlayerHUD::updateFunds(float passedFunds)
{
	PlayerFunds->SetText(FText::FromString(FString::SanitizeFloat(passedFunds)));
}