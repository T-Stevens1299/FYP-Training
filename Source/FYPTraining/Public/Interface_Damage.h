// Written by Thomas Stevens, all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Damage.generated.h"


UINTERFACE(MinimalAPI)
class UInterface_Damage : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FYPTRAINING_API IInterface_Damage
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void TriggerHealthCalculations();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void DealDamage(float damageDealt);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void SetHpTarget(UPARAM(ref)TArray<AActor*>& actorsToIgnore, AActor* currentTarget);
};
