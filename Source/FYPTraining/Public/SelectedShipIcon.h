// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectedShipIcon.generated.h"

class ASelectableObject;

UCLASS()
class FYPTRAINING_API USelectedShipIcon : public UUserWidget
{
	GENERATED_BODY()
public:
	USelectedShipIcon(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void init(ASelectableObject* shipRef, FString iconId, float percent);

	void changeHealth(float percent);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FString, UObject*> shipIcons;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* shipHealth;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* ShipIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ShipButton;

	UFUNCTION()
	void selectShipInIcon();

private:
	ASelectableObject* selectedShipRef;
};
