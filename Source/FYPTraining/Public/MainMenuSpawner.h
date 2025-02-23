// Written by Thomas Stevens, all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuSpawner.generated.h"

class UMainMenu;

UCLASS()
class FYPTRAINING_API AMainMenuSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	//Functions
	AMainMenuSpawner();

	//Variables
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> HUDref;

private:
	//References
	UMainMenu* HUD;

	APlayerController* PC;

protected:
	//Functions
	virtual void BeginPlay() override;
};