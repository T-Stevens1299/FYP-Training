// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectedShipsTab.generated.h"

class AFYPTrainingGameMode;
class USelectedShipIcon;

UCLASS()
class FYPTRAINING_API USelectedShipsTab : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USelectedShipsTab(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void init(AFYPTrainingGameMode* gameModeRef);

	void populateIcons();

	void updateHealthBar(int iconArrayIndex, float newHealthPercent);

	TArray<USelectedShipIcon*> shipIcons;

	UPROPERTY(EditDefaultsOnly, Category = "IconUI")
	TSubclassOf<UUserWidget> iconUiRef;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWrapBox* IconContainer;

private:
	AFYPTrainingGameMode* gmRef;
};