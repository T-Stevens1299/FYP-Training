// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectableObject.h"
#include "ResourceMine.generated.h"

class UBoxComponent;
class AFYPTrainingGameMode;
class AHardpoint;

UCLASS()
class FYPTRAINING_API AResourceMine : public ASelectableObject
{
	GENERATED_BODY()

public:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MineParts", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MineMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MineParts", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CaptureSensor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MineParts", meta = (AllowPrivateAccess = "true"))
	USceneComponent* hardpointSpawn;

	//Functions
	AResourceMine();

	void Init(AFYPTrainingGameMode* gamemodeReference);

	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/*void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/

	void generateIncome(float prevIncomeRate, bool techUpgrade);

	void buildMine();

	void captureMine();

	void captureMineSequence();

	void setMineLevel(int newMineLevel);

	void getMineLevel();

	virtual void HealthCalculations() override;

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineTweakables")
	float IncomeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineTweakables")
	TArray<float> IncomeRateLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineTweakables")
	float captureTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineTweakables")
	TSubclassOf<AActor> hardpointSpawnRef;

	AHardpoint* hardpointRef;

	bool isCaptured = false;

	bool isBuilt = false;

	bool firstCapture = true;

	float curCaptureTime;

	int enemyShipsCapturing;
	int playerShipsCapturing;

	//References
	AFYPTrainingGameMode* gmRef;

	APlayerController* PC;

	FTimerHandle incomeTimer;

	FTimerHandle captureTimer;

	FTimerHandle captureTriggerTimer;
};
