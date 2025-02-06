// Fill out your copyright notice in the Description page of Project Settings.


#include "AdminPanel.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include "Components/TextBlock.h"

UAdminPanel::UAdminPanel(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UAdminPanel::init(AFYPTrainingGameMode* gamemodeRef)
{
	gmRef = gamemodeRef;
	toggleVisibility();
	GetWorld()->GetTimerManager().SetTimer(updateStatTimer, this, &UAdminPanel::grabAiStats, 1.0f, true, 1.0f);
}

void UAdminPanel::toggleVisibility()
{
	if (isHidden) { this->SetVisibility(ESlateVisibility::Visible); isHidden = false; UE_LOG(LogTemp, Warning, TEXT("Visible")); }
	else { this->SetVisibility(ESlateVisibility::Hidden); isHidden = true; UE_LOG(LogTemp, Warning, TEXT("Hidden")); }
}

void UAdminPanel::grabAiStats()
{
	aiFunds = gmRef->currentAIMoney;
	aiIPS = gmRef->aiIncomePerSecond;
	aiDifficulty = gmRef->currentaiDifficulty;
	aiIncomeMultiplier = gmRef->aiIncomeMultiplier;
	aiPopCap = gmRef->currentAiPopCap;
	aiTechLevel = gmRef->aiTechLevel;
	setAiStats();
}

void UAdminPanel::setAiStats()
{
	Funds->SetText(FText::FromString(FString::FromInt(aiFunds)));
	IPS->SetText(FText::FromString(FString::FromInt(aiIPS)));
	Difficulty->SetText(FText::FromString(FString::SanitizeFloat(aiDifficulty)));
	IM->SetText(FText::FromString(FString::SanitizeFloat(aiIncomeMultiplier)));
	PopCount->SetText(FText::FromString(FString::FromInt(aiPopCap)));
	TechLevel->SetText(FText::FromString(FString::FromInt(aiTechLevel)));
}