// Fill out your copyright notice in the Description page of Project Settings.


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