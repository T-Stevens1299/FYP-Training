// Written by Thomas Stevens, all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class AFYPTrainingGameMode;

UCLASS()
class FYPTRAINING_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerHUD(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	//References
	AFYPTrainingGameMode* GMref;

	//Functions
	void updateFunds(float passedFunds);

	void SetGmPtr(AFYPTrainingGameMode* GMptr);

	UFUNCTION()
	void surrenderTheGame();

	void unlockSurrender();

protected:
	//UI components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* PlayerFunds;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* surrender;
};
