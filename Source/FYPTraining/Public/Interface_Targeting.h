// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Targeting.generated.h"


UINTERFACE(MinimalAPI)
class UInterface_Targeting : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FYPTRAINING_API IInterface_Targeting
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Targeting")
	void MoveToTarget(FVector TargetLocation, float AcceptanceRadius);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Targeting")
	void AttackTarget(AActor* Target);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Targeting")
	void AttackExistingTarget();

};
