// Written by Thomas Stevens, all rights reserved


//Class Deprecated due to memory leak crashes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface_Damage.h"
#include "Hardpoint.generated.h"

class ASelectableObject;

UCLASS()
class FYPTRAINING_API AHardpoint : public AActor, public IInterface_Damage
{
	GENERATED_BODY()
	
public:	

	AHardpoint();

protected:

	virtual void BeginPlay() override;

	//virtual void Tick(float DeltaTime) override;

	////Components
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HardpointParts", meta = (AllowPrivateAccess = "true"))
	//UStaticMeshComponent* HardpointMesh;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HardpointParts", meta = (AllowPrivateAccess = "true"))
	//USceneComponent* FirePoint;

	////Variables
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	//bool playerControlled;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	//bool canFire;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	//bool isWeapon;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	//float totalHealth;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	//float currentHealth;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	//float outputDamage;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	//float weaponRange;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	//float fireRate;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	//AHardpoint* targetRef;

	//UPROPERTY(BlueprintReadOnly)
	//ASelectableObject* hardpointParent;

	//AActor* currentHardpointTarget;

	//TArray<AActor*> ignoreActorsArray;

	//FColor playerDamageLine = FColor::Green;

	//FColor aiDamageLine = FColor::Red;

	//FColor usedDamageLine;

	//UFUNCTION(BlueprintNativeEvent)
	//AHardpoint* castToTarget(AActor* curTarget);

	////Functions
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	//void TriggerHealthCalculations(); void TriggerHealthCalculations_Implementation() override;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	//void DealDamage(float damageDealt); void DealDamage_Implementation(float damageDealt) override;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	//void SetHpTarget(UPARAM(ref)TArray<AActor*>& actorsToIgnore, AActor* currentTarget); void SetHpTarget_Implementation(UPARAM(ref)TArray<AActor*>& actorsToIgnore, AActor* currentTarget) override;

	//bool CheckTargetRange(TArray<AActor*> ActorsToIgnore);

	//void FireWeapon();

	//void SetTargetsParent();

	//void destroyHardpoint();

	//void setHealthAtStart(); //This is a workaround as instansiating hardpoints will destroy the actor you try to spawn one on.

	//UFUNCTION(BlueprintImplementableEvent)
	//void init();

	//FTimerHandle FireHandle;

};
