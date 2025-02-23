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
	currentUnitHealth = totalUnitHealth;
	playerControlled = player_controlled;
	unitCost = unit_Cost;
	PopulationValue = pop_Value;
	GetCharacterMovement()->MaxWalkSpeed = UnitSpeed;

	if (playerControlled) { usedDamageLine = playerDamageLine; }
	else { usedDamageLine = aiDamageLine; }

	//Init Functionality
	initBlueprintScript();

	GetWorldTimerManager().SetTimer(FireHandle, this, &ASelectableObject::fireBarrage, fireRate, true, fireRate);

	enemyShipSensor->SetSphereRadius(WeaponsRange);


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

	HealthCalculations(0);
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
		if (GetActorLocation() == retreatPointRef->GetActorLocation()) { return; }
		moveObject(retreatPointRef->GetActorLocation(), 500.0f);
		break;

	case 2:
		//Only issues order if target is not already in combat
		if (CurrentTarget == NULL)
		{
			if (!hasTarget) 
			{
				moveToAttackTarget(attackPointRef, 100);
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

	if (CurrentTarget) { moveToAttackTarget(CurrentTarget, WeaponsRange); }
}

void ASelectableObject::HealthCalculations(float passedDamage)
{
	if ((currentUnitHealth - passedDamage) >= 0)
	{
		currentUnitHealth = currentUnitHealth - passedDamage;
		healthBarRef->updateHealthBar(currentUnitHealth / totalUnitHealth);
	}
	else
	{
		triggerWinCheck();
	}
}

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

bool ASelectableObject::checkCurrentTargetInRange()
{
	TArray<AActor*> overlappingActors;
	enemyShipSensor->GetOverlappingActors(overlappingActors);
	for (int i = 0; i < overlappingActors.Num(); i++)
	{
		if (overlappingActors[i] == CurrentTarget) { return true; }
	}
	return false;
}

//Function stops the ship shooting a newly built credit mine if it was previously targeting the old one
void ASelectableObject::resetMineTarget()
{
	AResourceMine* checkMine = Cast<AResourceMine>(CurrentTarget);
	if (checkMine)
	{
		if (!checkMine->isBuilt)
		{
			CurrentTarget = NULL;
			/*if (playerControlled == checkMine->playerControlled) { CurrentTarget = NULL; }*/
		}
	}
}

void ASelectableObject::fireBarrage()
{
	if (!IsValid(CurrentTarget)) { CurrentTarget = NULL; return; }
	UE_LOG(LogTemp, Warning, TEXT("PassedCheck1"));
	if (checkTargetRange())
	{
		UE_LOG(LogTemp, Warning, TEXT("InRange"));
		ASelectableObject* curTarget = Cast<ASelectableObject>(CurrentTarget);
		if (!curTarget) { return; }
		curTarget->HealthCalculations(damageOutput);
	}
}

bool ASelectableObject::checkTargetRange()
{
	if (CurrentTarget == NULL) { return false; }

	FHitResult HitResult;
	FCollisionQueryParams params;

	FVector startLoc = GetActorLocation();
	FVector endLoc = CurrentTarget->GetActorLocation();

	params.AddIgnoredActor(this);

	if (!IsValid(CurrentTarget)) { return false; }
	UE_LOG(LogTemp, Warning, TEXT("PassedCheck2"));

	TArray<AActor*> actorsInRange;
	enemyShipSensor->GetOverlappingActors(actorsInRange);

	for (int i = 0; i < actorsInRange.Num(); i++)
	{
		if (actorsInRange[i] == CurrentTarget)
		{
			DrawDebugLine(GetWorld(), startLoc, endLoc, HitResult.GetActor() ? usedDamageLine : usedDamageLine, false, 1.0f, 0, 1.0f);
			return true;
		}
	}
	return false;
}

void ASelectableObject::locateEnemyInRange()
{
	//If the ship has no target, find one om weapons range and set it to current target

	if (hasTarget)
	{
		if (!checkCurrentTargetInRange()) { CurrentTarget = NULL; }
		return;
	}

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
						if (foundShip->playerControlled) { UE_LOG(LogTemp, Warning, TEXT("AIHasFoundTarget")); AttackTarget_Implementation(foundShip); break; }
					}
					else
					{
						if (!foundShip->playerControlled) { UE_LOG(LogTemp, Warning, TEXT("PlayerHasFoundTarget")); AttackTarget_Implementation(foundShip); break; }
					}
				}
			}
		}
	}
	else
	{
		resetMineTarget();
	}
}