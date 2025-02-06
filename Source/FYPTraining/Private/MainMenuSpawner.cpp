// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuSpawner.h"
#include "MainMenu.h"
#include "Blueprint/UserWidget.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AMainMenuSpawner::AMainMenuSpawner()
{

}

// Called when the game starts or when spawned
void AMainMenuSpawner::BeginPlay()
{
	Super::BeginPlay();

	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	HUD = CreateWidget<UMainMenu>(PC, HUDref);
	HUD->AddToViewport();
}