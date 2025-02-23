// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	AMainMenuSpawner();

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> HUDref;

private:
	UMainMenu* HUD;

	APlayerController* PC;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};