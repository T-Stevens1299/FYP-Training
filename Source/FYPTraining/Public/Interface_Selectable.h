// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Selectable.generated.h"


UINTERFACE(MinimalAPI)
class UInterface_Selectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FYPTRAINING_API IInterface_Selectable
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UnitSelect")
	void ToggleSelect(bool ToggleOn);

};
