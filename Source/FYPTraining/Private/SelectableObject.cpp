// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens


#include "SelectableObject.h"
#include "Hardpoint.h"
#include "Components/SphereComponent.h"


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

}

// Called when the game starts or when spawned
void ASelectableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASelectableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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


//Targeting Functions

void ASelectableObject::MoveToTarget_Implementation(FVector TargetLocation, float AcceptanceRadius)
{
	if (!staticObject)
	{
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

	if (ASelectableObject::CurrentTarget)
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
		CurrentShipTarget = NULL;
	}
}

void ASelectableObject::selectHardpointToTarget()
{
	ASelectableObject* targetRef;

	targetRef = Cast<ASelectableObject>(CurrentTarget);

	if (targetRef)
	{
		CurrentTarget = targetRef->Hardpoints.Last();		
		moveToAttackTarget(CurrentTarget, WeaponsRange);
	}
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
		for (int i = 0; i < Hardpoints.Num(); i++)
		{
			if (Hardpoints.IsValidIndex(i))
			{
				tempHardpoint = Cast<AHardpoint>(Hardpoints[i]);

				if (tempHardpoint)
				{
					tempHardpoint->hardpointParent = this;
					tempHardpoint->playerControlled = playerControlled;
					tempHardpoint->init();
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
		UE_LOG(LogTemp, Warning, TEXT("The Total unit health is: %d"), (int)newTotalHealth);
	}

	currentUnitHealth = newTotalHealth;

	if (newTotalHealth <= 0)
	{
		//TriggerWinCheck
		this->Destroy(true);
	}
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
}