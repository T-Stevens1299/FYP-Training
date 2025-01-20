// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_GetUnitOrderCode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SelectableObject.h"

UBTS_GetUnitOrderCode::UBTS_GetUnitOrderCode()
{
	NodeName = "Get Order Code";
}

void UBTS_GetUnitOrderCode::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UE_LOG(LogTemp, Warning, TEXT("BT Ran"));
	AActor* TargetPosition = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetPositionKey.SelectedKeyName));
	AActor* SelfActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfActorKey.SelectedKeyName));

	if (SelfActor && TargetPosition)
	{
		ASelectableObject* shipRef = Cast<ASelectableObject>(SelfActor);
		if (shipRef)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(OrderCodeKey.SelectedKeyName, shipRef->orderCode);
			UE_LOG(LogTemp, Warning, TEXT("BT Success"));
		}
	}
}