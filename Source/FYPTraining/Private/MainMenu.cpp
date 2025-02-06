// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UMainMenu::NativeConstruct()
{
	if (LevelSelect) { LevelSelect->OnClicked.AddDynamic(this, &UMainMenu::openLevelSelect); }
	if (Quit) { Quit->OnClicked.AddDynamic(this, &UMainMenu::exitGame); }
	if (BasicAI) { BasicAI->OnClicked.AddDynamic(this, &UMainMenu::playBasicAI); }
	if (CheatingAI) { CheatingAI->OnClicked.AddDynamic(this, &UMainMenu::playCheatingAI); }
	if (LanchesterAI) { LanchesterAI->OnClicked.AddDynamic(this, &UMainMenu::playLanchesterAI); }
	if (DDSAI) { DDSAI->OnClicked.AddDynamic(this, &UMainMenu::playDdsAI); }
	if (HardestAI) { HardestAI->OnClicked.AddDynamic(this, &UMainMenu::playHardestAI); }
	if (BackToMenu) { BackToMenu->OnClicked.AddDynamic(this, &UMainMenu::returnToMenu); }

	returnToMenu();
}

void UMainMenu::openLevelSelect()
{
	MenuScreen->SetVisibility(ESlateVisibility::Hidden);
	LevelSelectScreen->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenu::exitGame()
{
	TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit; UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), QuitPreference, true);
}

void UMainMenu::playBasicAI()
{
	UGameplayStatics::OpenLevel(this, BasicAiMap);
}

void UMainMenu::playCheatingAI()
{
	UGameplayStatics::OpenLevel(this, CheatingAiMap);
}

void UMainMenu::playLanchesterAI()
{
	UGameplayStatics::OpenLevel(this, LanchesterAiMap);
}

void UMainMenu::playDdsAI()
{
	UGameplayStatics::OpenLevel(this, DdsAiMap);

}

void UMainMenu::playHardestAI()
{
	UGameplayStatics::OpenLevel(this, HardAiMap);
}

void UMainMenu::returnToMenu()
{
	MenuScreen->SetVisibility(ESlateVisibility::Visible);
	LevelSelectScreen->SetVisibility(ESlateVisibility::Hidden);
}