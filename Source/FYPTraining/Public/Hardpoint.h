// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens


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

public:	

	virtual void Tick(float DeltaTime) override;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HardpointParts", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HardpointMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HardpointParts", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BlockingPlane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HardpointParts", meta = (AllowPrivateAccess = "true"))
	USceneComponent* FirePoint;

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	bool playerControlled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	bool canFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	bool isWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	float totalHealth;

	float currentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	float outputDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	float weaponRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HardpointTweakables")
	float fireRate;

	UPROPERTY(BlueprintReadOnly)
	ASelectableObject* hardpointParent;

	AActor* currentHardpointTarget;

	//Functions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void TriggerHealthCalculations(); void TriggerHealthCalculations_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void DealDamage(float damageDealt); void DealDamage_Implementation(float damageDealt) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void SetHpTarget(UPARAM(ref)TArray<AActor*>& actorsToIgnore, AActor* currentTarget); void SetHpTarget_Implementation(UPARAM(ref)TArray<AActor*>& actorsToIgnore, AActor* currentTarget) override;

	bool CheckTargetRange(TArray<AActor*> ActorsToIgnore);

	void FireWeapon();

	void SetTargetsParent();

	UFUNCTION(BlueprintImplementableEvent)
	void init();

	FTimerHandle FireHandle;

};
