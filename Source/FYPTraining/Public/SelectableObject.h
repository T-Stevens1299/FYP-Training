// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface_Selectable.h"
#include "Interface_Targeting.h"
#include "Interface_Damage.h"
#include "SelectableObject.generated.h"

class USphereComponent;
class AHardpoint;
class UBehaviorTree;
class UShipHealthBar;
class UWidgetComponent;

UCLASS()
class FYPTRAINING_API ASelectableObject : public ACharacter, public IInterface_Selectable, public IInterface_Targeting, public IInterface_Damage
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASelectableObject();

	//Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreParts", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* UnitMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreParts", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SelectorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreParts", meta = (AllowPrivateAccess = "true"))
	USphereComponent* ClickDetector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreParts", meta = (AllowPrivateAccess = "true"))
	USphereComponent* enemyShipSensor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreParts", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* healthBar;
	//Functions

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UnitSelect")
	void ToggleSelect(bool ToggleOn); void ToggleSelect_Implementation(bool ToggleOn) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Targeting")
	void MoveToTarget(FVector TargetLocation, float AcceptanceRadius); void MoveToTarget_Implementation(FVector TargetLocation, float AcceptanceRadius) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Targeting")
	void AttackTarget(AActor* Target); 	void AttackTarget_Implementation(AActor* Target) override;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Targeting")
	//void AttackExistingTarget(); 	void AttackExistingTarget_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Targeting")
	void moveToAttackTarget(AActor* target, float acceptanceRange);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Setup")
	void moveObject(FVector Location, float acceptanceRange); 

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Setup")
	void initBlueprintScript();

	virtual void HealthCalculations(float passedDamage);

	void initaliseSelectableObject(bool player_controlled, float unit_Cost, int pop_Value);

	void initialiseAIShips();

	virtual void triggerWinCheck();

	//Health Percentage getter
	float getUnitHealthPercentage() { return (currentUnitHealth / totalUnitHealth); }

	float getStrengthValue() { return (unitCost * currentUnitHealth) / totalUnitHealth; }

	//Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	bool hasTarget = false;

	bool initHealthCheck = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	bool staticObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	bool playerControlled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	float totalUnitHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	float currentUnitHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	float damageOutput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	float fireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	float unitCost;
	
	//Editable Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	float UnitSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	float UnitRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	int PopulationValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	int orderCode;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	float WeaponsRange;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	AActor* retreatPointRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	AActor* attackPointRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	AActor* CurrentTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	UMaterial* playerMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	UMaterial* enemyMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	UMaterialInstance* playerTexture;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	UShipHealthBar* healthBarRef;

protected:
	virtual void BeginPlay() override;

private:
	void checkOrderCode();

	void locateEnemyInRange();

	void SetMaterials();

	bool checkCurrentTargetInRange();

	void resetMineTarget();

	void fireBarrage();

	bool checkTargetRange();

	FTimerHandle behaviourTreeTick;

	FTimerHandle enemySensorTimer;

	FTimerHandle FireHandle;

	FColor playerDamageLine = FColor::Green;

	FColor aiDamageLine = FColor::Red;

	FColor usedDamageLine;
};
