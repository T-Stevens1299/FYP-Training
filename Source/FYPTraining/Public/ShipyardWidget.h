// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShipyardWidget.generated.h"

/**
 * 
 */
UCLASS()
class FYPTRAINING_API UShipyardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//Functions
	UShipyardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	//Variables

	//Components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildCorvette;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildFrigate;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildDestroyer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildCruiser;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildBattleship;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildAircraftCarrier;



	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* TechLevel2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* TechLevel3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* TechLevel4;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* HealthUpgrade1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* HealthUpgrade2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* HealthUpgrade3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* HealthUpgrade4;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* DamageUpgrade1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* DamageUpgrade2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* DamageUpgrade3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* DamageUpgrade4;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* AircraftCarrierUpgrade;
};
