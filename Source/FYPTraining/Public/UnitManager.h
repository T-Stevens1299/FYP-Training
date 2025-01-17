// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitManager.generated.h"

class AShipyard;
struct FConstructionData;
class AFYPTrainingGameMode;

UCLASS()
class FYPTRAINING_API AUnitManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitManager();

	void Init(AFYPTrainingGameMode* gmRef);

	void setShipyardPtr(AShipyard* shipyardRef);

	void upgradeTechLevel(FConstructionData* currentRowRef);
	
	AShipyard* shipYardRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle dataTableRef;

	FConstructionData* currentRow;

	AFYPTrainingGameMode* gamemodeRef;

	//Variables
	int currentTechLevel = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
