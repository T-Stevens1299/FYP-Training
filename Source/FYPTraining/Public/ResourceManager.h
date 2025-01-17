// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceManager.generated.h"

class AFYPTrainingGameMode;
class ACombatManager;

UCLASS()
class FYPTRAINING_API AResourceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceManager();

	void Init(AFYPTrainingGameMode* gmRef);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AFYPTrainingGameMode* gamemodeRef;

	TSubclassOf<AActor> combatManager;
	
	ACombatManager* cmRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineProperties")
	AActor* mine1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineProperties")
	AActor* mine2;
};
