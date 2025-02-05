// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens


#include "SelectableObject.h"
#include "Hardpoint.h"
#include "Components/SphereComponent.h"
#include <Kismet/GameplayStatics.h>
#include "FYPTraining/FYPTrainingGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ResourceMine.h"
#include "Components/WidgetComponent.h"
#include "ShipHealthBar.h"
#include "Components/ProgressBar.h"


// Sets default values
ASelectableObject::ASelectableObject()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UnitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UnitMeshComponent"));
	UnitMesh->SetupAttachment(RootComponent);

	SelectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectorMeshComponent"));
	SelectorMesh->SetupAttachment(RootComponent);

	ClickDetector = CreateDefaultSubobject<USphereComponent>(TEXT("ClickDetectorComponent"));
	ClickDetector->SetupAttachment(RootComponent);

	enemyShipSensor = CreateDefaultSubobject<USphereComponent>(TEXT("ShipSensorComponent"));
	enemyShipSensor->SetupAttachment(RootComponent);

	healthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("ShipHealthBarComponent"));
	healthBar->SetupAttachment(RootComponent);

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, UnitRotationSpeed);
}

void ASelectableObject::BeginPlay()
{
	Super::BeginPlay();
	healthBarRef = Cast<UShipHealthBar>(healthBar->GetUserWidgetObject());
}

//Function only used on ship classes. Resource mine and shipyards
//Despite all being children of this class use their own init methods
void ASelectableObject::initaliseSelectableObject(bool player_controlled, float unit_Cost, int pop_Value)
{
	//Init Variables
	playerControlled = player_controlled;
	unitCost = unit_Cost;
	PopulationValue = pop_Value;
	GetCharacterMovement()->MaxWalkSpeed = UnitSpeed;

	//Init Functionality
	initBlueprintScript();
	setHardpointsParent();


	//Sets enemy sensor timer up
	GetWorldTimerManager().SetTimer(enemySensorTimer, this, &ASelectableObject::locateEnemyInRange, 2, true, 2);

	if (!playerControlled) 
	{ 
		healthBarRef->HealthBar->SetFillColorAndOpacity(FLinearColor::Red);
		initialiseAIShips(); 
	}
	else
	{
		healthBarRef->HealthBar->SetFillColorAndOpacity(FLinearColor::Green);
		SetMaterials();
	}
}

void ASelectableObject::initialiseAIShips() //Only initialise on enemy ships - function name should be changed to reflect this
{
	GetWorldTimerManager().SetTimer(behaviourTreeTick, this, &ASelectableObject::checkOrderCode, 2, true, 2);
}

//Only player ships require material changes, the blueprint defaults to having the AI colours
void ASelectableObject::SetMaterials()
{
	if (UnitMesh->GetMaterials().Num() > 1)
	{
		//Two ship types feature models that have more than 1 matreial slot - this code accounts for that
		if (IsValid(UnitMesh->GetMaterial(2))) { UnitMesh->SetMaterial(2, playerMaterial); }
		if (IsValid(UnitMesh->GetMaterial(3))) { UnitMesh->SetMaterial(3, playerMaterial); }
	}
	else
	{
		UnitMesh->SetMaterial(0, playerTexture);
	}
}

// Called to bind functionality to input
void ASelectableObject::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASelectableObject::ToggleSelect_Implementation(bool ToggleOn)
{
	SelectorMesh->SetVisibility(ToggleOn);
}

void ASelectableObject::checkOrderCode()
{
	switch (orderCode)
	{
	case 0:
		break;

	case 1:
		moveObject(retreatPointRef->GetActorLocation(), 500.0f);
		break;

	case 2:
		if (CurrentTarget == NULL)
		{
			if (!hasTarget) 
			{
				MoveToTarget(attackPointRef->GetActorLocation(), 100);
			}
		}
		break;

	default:
		break;
	}
}

//Targeting Functions

void ASelectableObject::MoveToTarget_Implementation(FVector TargetLocation, float AcceptanceRadius)
{
	if (!staticObject)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actor location %s"), *TargetLocation.ToCompactString());
		moveObject(TargetLocation, AcceptanceRadius);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot move object is static"));
	}
}

void ASelectableObject::AttackTarget_Implementation(AActor* Target)
{
	CurrentTarget = Target;

	if (CurrentTarget)
	{
		selectHardpointToTarget();
	}
	else
	{
		moveToAttackTarget(CurrentTarget, WeaponsRange);
	}
}

void ASelectableObject::AttackExistingTarget_Implementation()
{
	if (IsValid(CurrentShipTarget))
	{
		CurrentTarget = CurrentShipTarget;
		selectHardpointToTarget();
	}
	else
	{
		CurrentTarget = NULL;
		CurrentShipTarget = NULL;
	}
}

void ASelectableObject::selectHardpointToTarget()
{
	ASelectableObject* targetRef;

	targetRef = Cast<ASelectableObject>(CurrentTarget);

	if (!targetRef) { return; }
	CurrentTarget = targetRef->Hardpoints.Last();	
	if(!CurrentTarget) { return; }
	moveToAttackTarget(CurrentTarget, WeaponsRange);
}

void ASelectableObject::setHardpointTarget()
{
	for (int i = 0; i < Hardpoints.Num(); i++)
	{
		if (Hardpoints.IsValidIndex(i))
		{
			if (Hardpoints[i]->GetClass()->ImplementsInterface(UInterface_Damage::StaticClass()))
			{
				IInterface_Damage::Execute_SetHpTarget(Hardpoints[i], Hardpoints, CurrentTarget);
			}
		}
	}
}

//Hardpoint and class setup Functions

void ASelectableObject::setHardpointsParent()
{

	TArray<AActor*> childActors;
	GetAllChildActors(childActors);
	UE_LOG(LogTemp, Warning, TEXT("Hardpoints In Array: %d"), (int)childActors.Num());

	for (int LosingMyEdge = 0; LosingMyEdge < childActors.Num(); LosingMyEdge++)
	{
		AHardpoint* validHardpoint = Cast<AHardpoint>(childActors[LosingMyEdge]);
		if (validHardpoint)
		{
			//Adds prospective hardpoints to the array then initialises it
			Hardpoints.Add(validHardpoint);

			validHardpoint->hardpointParent = this;
			validHardpoint->playerControlled = playerControlled;
			validHardpoint->init();
			if (playerControlled)
			{
				validHardpoint->usedDamageLine = validHardpoint->playerDamageLine;
			}
			else
			{
				validHardpoint->usedDamageLine = validHardpoint->aiDamageLine;
			}
		}
	}

	calculateWeaponsRange();
	HealthCalculations();
}

void ASelectableObject::TriggerHealthCalculations_Implementation()
{
	HealthCalculations();
}

void ASelectableObject::HealthCalculations()
{
	float newTotalHealth = 0.0f;
	AHardpoint* curHardpointRef;

	for (int i = 0; i < Hardpoints.Num(); i++)
	{
		curHardpointRef = Cast<AHardpoint>(Hardpoints[i]);
		if (curHardpointRef)
		{
			newTotalHealth = newTotalHealth + curHardpointRef->currentHealth;
		}
	}

	if (initHealthCheck)
	{
		totalUnitHealth = newTotalHealth;
		initHealthCheck = false;
	}

	currentUnitHealth = newTotalHealth;
	
	float percent = currentUnitHealth / totalUnitHealth;

	if (IsValid(healthBarRef)) { healthBarRef->updateHealthBar(percent); }

	if (newTotalHealth <= 0)
	{
		triggerWinCheck();
	}
}

//Function only exists to override in shipyard class
void ASelectableObject::triggerWinCheck()
{
	AFYPTrainingGameMode* gmRef = Cast<AFYPTrainingGameMode>(UGameplayStatics::GetGameMode(this));
	if (gmRef)
	{
		gmRef->updatePopCap(playerControlled, -PopulationValue);
		if (playerControlled)
		{
			gmRef->removeShipsFromArray(this, GetClass()->GetName(), playerControlled);
		}
		else
		{
			gmRef->removeShipsFromArray(this, GetClass()->GetName(), playerControlled);
		}
	}
	this->Destroy(true);
}

void ASelectableObject::calculateWeaponsRange()
{
	//Calculates the ships weapon range - determined by the lowest weapon range of all hardpoints. This becomes the acceptance radius when attacking a target so all weapons can fire
	AHardpoint* currentHardpointRef;

	TArray<float> hardpointWeaponRanges;

	for (int i = 0; i < Hardpoints.Num(); i++)
	{
		currentHardpointRef = Cast<AHardpoint>(Hardpoints[i]);
		if (currentHardpointRef)
		{
			if (currentHardpointRef->isWeapon)
			{
				hardpointWeaponRanges.Add(currentHardpointRef->weaponRange);
			}
		}
	}

	hardpointWeaponRanges.Sort();
	
	if (hardpointWeaponRanges.IsValidIndex(0))
	{
		WeaponsRange = hardpointWeaponRanges[0];
	}

	enemyShipSensor->SetSphereRadius(WeaponsRange);
}

void ASelectableObject::locateEnemyInRange()
{
	//If Ship has the order code of 1, it is retreating and should not be seeking new targets
	if (orderCode == 1) { return; }

	//If the ship has no target, find one om weapons range and set it to current target
	if (CurrentTarget == NULL)
	{
		TArray<AActor*> overlappingActors;
		enemyShipSensor->GetOverlappingActors(overlappingActors);
		for (int meanGirls = 0; meanGirls < overlappingActors.Num(); meanGirls++)
		{
			ASelectableObject* foundShip = Cast<ASelectableObject>(overlappingActors[meanGirls]);
			if (foundShip) 
			{
				AResourceMine* mineRef = Cast<AResourceMine>(foundShip);
				if (mineRef) 
				{ 
					if (mineRef->isBuilt == false) { UE_LOG(LogTemp, Warning, TEXT("UnCapturedMineNotValidToAttack")); return; } 
				}
				else
				{
					//if the ship is player controlled target non-player controlled ships and vice versa
					if (!playerControlled)
					{
						if (foundShip->playerControlled) { UE_LOG(LogTemp, Warning, TEXT("FoundTarget")); AttackTarget_Implementation(foundShip); break; }
					}
					else
					{
						if (!foundShip->playerControlled) { UE_LOG(LogTemp, Warning, TEXT("FoundTarget")); AttackTarget_Implementation(foundShip); break; }
					}
				}
			}
		}
	}
}