// Written by Thomas Stevens, all rights reserved


#include "GameEndScreen.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

UGameEndScreen::UGameEndScreen(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UGameEndScreen::NativeConstruct()
{
	if (BackToMenu)
	{
		BackToMenu->OnClicked.AddDynamic(this, &UGameEndScreen::returnToMenu);
	}
}

void UGameEndScreen::SetScreenText(bool victor)
{
	pCorLoss->SetText(pCorvetteLoss);
	aiCorLoss->SetText(aiCorvetteLoss);

	pFrigLoss->SetText(pFrigateLoss);
	aiFrigLoss->SetText(aiFrigateLoss);

	pDestroyLoss->SetText(pDestroyerLoss);
	aiDestroyLoss->SetText(aiDestroyerLoss);

	pCruiseLoss->SetText(pCruiserLoss);
	aiCruiseLoss->SetText(aiCruiserLoss);

	pBattleLoss->SetText(pBattleShipLoss);
	aiBattleLoss->SetText(aiBattleShipLoss);

	pTotalLoss->SetText(pLoss);
	aiTotalLoss->SetText(aiLoss);

	int matchMinutes = matchLength / 60;
	int matchSeconds = matchLength % 60;

	FString matchLengthString = FString::FromInt(matchMinutes) + ":" + FString::FromInt(matchSeconds);

	GameDuration->SetText(FText::FromString(matchLengthString));
}

void UGameEndScreen::returnToMenu()
{
	UGameplayStatics::OpenLevel(this, MainMenu);
}