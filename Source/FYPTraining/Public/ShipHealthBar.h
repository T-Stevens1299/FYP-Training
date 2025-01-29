// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShipHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class FYPTRAINING_API UShipHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UShipHealthBar(const FObjectInitializer& ObjectInitializer);

	void updateHealthBar(float passedPercentage);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar;
	
};
