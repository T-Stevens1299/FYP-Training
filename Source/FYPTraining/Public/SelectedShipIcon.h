// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectedShipIcon.generated.h"

/**
 * 
 */
UCLASS()
class FYPTRAINING_API USelectedShipIcon : public UUserWidget
{
	GENERATED_BODY()
public:
	USelectedShipIcon(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void init(FString iconId, float percent);

	void changeHealth(float percent);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FString, UTexture2D*> shipIcons;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* shipHealth;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ShipIcon;
};
