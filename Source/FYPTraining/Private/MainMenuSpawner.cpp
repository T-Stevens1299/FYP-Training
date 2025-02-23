// Written by Thomas Stevens, all rights reserved

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
	HUD->init(PC);
	HUD->AddToViewport();
}