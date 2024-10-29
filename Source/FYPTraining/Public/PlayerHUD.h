// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens

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
