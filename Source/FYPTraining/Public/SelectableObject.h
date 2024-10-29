// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface_Selectable.h"
#include "SelectableObject.generated.h"

class USphereComponent;

UCLASS()
class FYPTRAINING_API ASelectableObject : public ACharacter, public IInterface_Selectable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASelectableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreParts", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* UnitMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreParts", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SelectorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CoreParts", meta = (AllowPrivateAccess = "true"))
	USphereComponent* ClickDetector;

	//Functions
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UnitSelect")
	void ToggleSelect(bool ToggleOn); void ToggleSelect_Implementation(bool ToggleOn) override;

	//Variables
	//declare hardpoint array
	bool hasTarget;

	bool isEnemy;
	
	//Editable Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	float UnitSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	float UnitRotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	int PopulationValue;

	//Weapons Range variable decided by lowest hardpoint weapons range
	float WeaponsRange;

	AActor* CurrentTarget;

	AActor* CurrentShipTarget;

};
