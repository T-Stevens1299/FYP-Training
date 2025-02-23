// Written by Thomas Stevens, all rights reserved

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

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Setup")
	void unbindBlueprintEvents();

	void Init(AFYPTrainingGameMode* gamemodeReference);

	void buildMine();

	void setMineLevel(int newMineLevel);

	virtual void HealthCalculations(float passedDamage) override;

	//Variables
	bool isBuilt = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineTweakables")
	float IncomeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineTweakables")
	TArray<float> IncomeRateLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineTweakables")
	float captureTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineTweakables")
	TSubclassOf<AActor> hardpointSpawnRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	UMaterial* mineMaterial;

private:
	//Variables
	bool isCaptured = false;

	bool firstCapture = true;

	float curCaptureTime;

	int enemyShipsCapturing;

	int playerShipsCapturing;

	//References
	AFYPTrainingGameMode* gmRef;

	APlayerController* PC;

	FTimerHandle delayTimer;

	FTimerHandle captureTimer;

	FTimerHandle captureTriggerTimer;

	//Functions
	void resetMine();

	void captureMine();

	void captureMineSequence();

	void generateIncome(float newIncome, bool techUpgrade);
};
