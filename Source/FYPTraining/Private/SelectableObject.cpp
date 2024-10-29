// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens


#include "SelectableObject.h"
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
	ClickDetector->SetVisibility(ToggleOn);
}
