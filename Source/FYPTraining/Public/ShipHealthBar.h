// Written by Thomas Stevens, all rights reserved

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

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar;	
};
