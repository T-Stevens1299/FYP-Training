// Written by Thomas Stevens, all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndScreen.generated.h"

UCLASS()
class FYPTRAINING_API UGameEndScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	//Functions
	UGameEndScreen(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetScreenText(bool victor);
	
	//Variables
	FText pCorvetteLoss;
	FText aiCorvetteLoss;

	FText pFrigateLoss;
	FText aiFrigateLoss;

	FText pDestroyerLoss;
	FText aiDestroyerLoss;

	FText pCruiserLoss;
	FText aiCruiserLoss;

	FText pBattleShipLoss;
	FText aiBattleShipLoss;

	FText pLoss;
	FText aiLoss;

	int matchLength;

protected:
	UFUNCTION()
	void returnToMenu();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* pCorLoss;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* aiCorLoss;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* pFrigLoss;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* aiFrigLoss;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* pDestroyLoss;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* aiDestroyLoss;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* pCruiseLoss;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* aiCruiseLoss;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* pBattleLoss;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* aiBattleLoss;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* pTotalLoss;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* aiTotalLoss;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* GameDuration;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BackToMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MenuNameRef")
	FName MainMenu;
};