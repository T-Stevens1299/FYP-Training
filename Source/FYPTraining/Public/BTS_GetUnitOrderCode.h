// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_GetUnitOrderCode.generated.h"

/**
 * 
 */
UCLASS()
class FYPTRAINING_API UBTS_GetUnitOrderCode : public UBTService
{
	GENERATED_BODY()

	UBTS_GetUnitOrderCode();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetPositionKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector OrderCodeKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector SelfActorKey;
};
