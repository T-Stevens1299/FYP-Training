// Written by Thomas Stevens, all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;
class UCanvasPanel;

UCLASS()
class FYPTRAINING_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void init(APlayerController* pcRef);

protected:
	//Button Functions
	UFUNCTION()
	void openLevelSelect();

	UFUNCTION()
	void exitGame();

	UFUNCTION()
	void playBasicAI();

	UFUNCTION()
	void playCheatingAI();

	UFUNCTION()
	void playLanchesterAI();

	UFUNCTION()
	void playDdsAI();

	UFUNCTION()
	void playHardestAI();

	UFUNCTION()
	void returnToMenu();


	//Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* LevelSelect;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Quit;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BasicAI;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* CheatingAI;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* LanchesterAI;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DDSAI;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* HardestAI;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BackToMenu;

	//Canvas Panels
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* MenuScreen;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* LevelSelectScreen;

	//Level Names
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelNames")
	FName BasicAiMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelNames")
	FName CheatingAiMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelNames")
	FName LanchesterAiMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelNames")
	FName DdsAiMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelNames")
	FName HardAiMap;
private:
	APlayerController* PC;
};
