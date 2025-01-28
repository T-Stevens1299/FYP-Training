// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens



#include "Hardpoint.h"
#include "SelectableObject.h"


AHardpoint::AHardpoint()
{

	PrimaryActorTick.bCanEverTick = true;

	HardpointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HardpointMeshComponent"));
	HardpointMesh->SetupAttachment(RootComponent);

	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("UnitMeshComponent"));
	FirePoint->SetupAttachment(HardpointMesh);

}


void AHardpoint::BeginPlay()
{
	Super::BeginPlay();
	currentHealth = totalHealth;
	GetWorldTimerManager().SetTimer(FireHandle, this, &AHardpoint::FireWeapon, fireRate, true, 0.0f);
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
	FTimerHandle temp;
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
						hardpointParent->Hardpoints.RemoveSingle(hardpointParent->Hardpoints[i]);
						hardpointParent->HealthCalculations();
						destroyHardpoint();
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

void AHardpoint::destroyHardpoint()
{
	this->Destroy(true);
}

void AHardpoint::SetHpTarget_Implementation(UPARAM(ref)TArray<AActor*>& actorsToIgnore, AActor* currentTarget)
{
	if (isWeapon)
	{
		currentHardpointTarget = currentTarget;
		ignoreActorsArray = actorsToIgnore;
	}
}

void AHardpoint::FireWeapon()
{
	if (CheckTargetRange(ignoreActorsArray))
	{
		if (currentHardpointTarget->GetClass()->ImplementsInterface(UInterface_Damage::StaticClass()))
		{
			IInterface_Damage::Execute_DealDamage(currentHardpointTarget, outputDamage);
			UE_LOG(LogTemp, Warning, TEXT("Fire"))
		}
	}
	else
	{
		IInterface_Targeting::Execute_AttackExistingTarget(hardpointParent);
	}
}

bool AHardpoint::CheckTargetRange(TArray<AActor*> ActorsToIgnore)
{
	//Local Variables
	if (currentHardpointTarget == NULL) { return false; }
	bool canShoot;

	FHitResult HitResult;
	FCollisionQueryParams params;

	FVector startLoc = FirePoint->GetComponentLocation();
	FVector endLoc = currentHardpointTarget->GetActorLocation();

	params.AddIgnoredActors(ActorsToIgnore);

	if (IsValid(currentHardpointTarget))
	{
		SetTargetsParent();

		if (GetWorld()->LineTraceSingleByChannel(HitResult, startLoc, endLoc, ECollisionChannel::ECC_Visibility, params, FCollisionResponseParams()))
		{
			DrawDebugLine(GetWorld(), startLoc, endLoc, HitResult.GetActor() ? usedDamageLine : usedDamageLine, false, 2.0f, 0, 2.0f);
			
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