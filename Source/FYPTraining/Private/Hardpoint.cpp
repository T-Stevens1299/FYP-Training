// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens



#include "Hardpoint.h"
#include "SelectableObject.h"


AHardpoint::AHardpoint()
{

	PrimaryActorTick.bCanEverTick = true;

	HardpointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HardpointMeshComponent"));
	HardpointMesh->SetupAttachment(RootComponent);

	BlockingPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockingPlaneComponent"));
	BlockingPlane->SetupAttachment(RootComponent);

	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("UnitMeshComponent"));
	FirePoint->SetupAttachment(HardpointMesh);

}


void AHardpoint::BeginPlay()
{
	Super::BeginPlay();
	
	currentHealth = totalHealth;
}


void AHardpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHardpoint::TriggerHealthCalculations_Implementation()
{

}

void AHardpoint::DealDamage_Implementation(float damageDealt)
{
	float tempHealth = currentHealth - damageDealt;

	if (tempHealth <= 0)
	{
		//Removes hardpoint from the array before destroying it
		if (hardpointParent)
		{
			for (int i = 0; i < hardpointParent->Hardpoints.Num(); i++)
			{
				if (hardpointParent->Hardpoints.IsValidIndex(i))
				{
					if (hardpointParent->Hardpoints[i] == this)
					{
						hardpointParent->Hardpoints.Remove(hardpointParent->Hardpoints[i]);
						hardpointParent->HealthCalculations();
						this->Destroy(true);
					}
				}
			}
		}
	}
	else
	{
		//Health recalculations
		currentHealth = tempHealth;
		if (hardpointParent)
		{
			hardpointParent->HealthCalculations();
		}
	}
}

void AHardpoint::SetHpTarget_Implementation(UPARAM(ref)TArray<AActor*>& actorsToIgnore, AActor* currentTarget)
{
	if (isWeapon)
	{
		currentHardpointTarget = currentTarget;
		if (CheckTargetRange(actorsToIgnore))
		{
			GetWorldTimerManager().SetTimer(FireHandle, this, &AHardpoint::FireWeapon, 0.0f, true, fireRate);
		}
	}
}

void AHardpoint::FireWeapon()
{
	if (currentHardpointTarget)
	{
		DealDamage(outputDamage);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(FireHandle);
		//Attack Exisitng Target - pass parent unit
	}
}

bool AHardpoint::CheckTargetRange(TArray<AActor*> ActorsToIgnore)
{
	//Local Variables
	bool canShoot;

	FHitResult HitResult;
	FCollisionQueryParams params;

	FVector startLoc = FirePoint->GetComponentLocation();
	FVector endLoc = currentHardpointTarget->GetActorLocation();

	//for (int i = 0; i < ignore.Num(); i++)
	//{
	//	if (ignore.IsValidIndex(i))
	//	{
	//		if (ignore[i] = this)
	//		{
	//			ignore.Remove(ignore[i]);
	//		}
	//	}
	//}
	params.AddIgnoredActors(ActorsToIgnore);

	if (currentHardpointTarget)
	{
		SetTargetsParent();

		if (GetWorld()->LineTraceSingleByChannel(HitResult, startLoc, endLoc, ECollisionChannel::ECC_Visibility, params, FCollisionResponseParams()))
		{
			if (HitResult.Distance <= weaponRange)
			{
				canShoot = true;
			}
			else
			{
				canShoot = false;
			}
		}

	}
	else
	{
		GetWorldTimerManager().ClearTimer(FireHandle);
	}

	return canShoot;
}

void AHardpoint::SetTargetsParent()
{
	AHardpoint* targetRef = Cast<AHardpoint>(currentHardpointTarget);
	if (targetRef)
	{
		hardpointParent->CurrentShipTarget = targetRef->hardpointParent;
	}
}