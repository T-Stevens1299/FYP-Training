// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class FYPTRAINING_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerHUD(const FObjectInitializer& ObjectInitializer);

public:
	//Variables
	class AFYPTrainingGameMode* GMref;

	//Functions
	void updateFunds(float passedFunds);

	void SetGmPtr(AFYPTrainingGameMode* GMptr);

protected:
	//UI components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* PlayerFunds;
};
