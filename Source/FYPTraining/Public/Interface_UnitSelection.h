// Written by Thomas Stevens, all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_UnitSelection.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_UnitSelection : public UInterface
{
	GENERATED_BODY()
};

class FYPTRAINING_API IInterface_UnitSelection
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UnitSelection")
	void OnHover(bool playerControlled, AActor* hoveredActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UnitSelection")
	void ClearHover();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UnitSelection")
	void RegisterUnit(AActor* SelectedActor);
};
