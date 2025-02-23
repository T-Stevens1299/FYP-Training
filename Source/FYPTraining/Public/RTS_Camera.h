// Written by Thomas Stevens, all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "RTS_Camera.generated.h"

class UInputAction;
class UInputMappingContext;
class AFYPTrainingGameMode;
struct FInputActionValue;

UCLASS()
class FYPTRAINING_API ARTS_Camera : public APawn
{
	GENERATED_BODY()

public:	
	//Functions
	ARTS_Camera();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ZoomCamera(const FInputActionValue& Value);

	void MoveCamera(const FInputActionValue& Value);

	void RotateCamera(const FInputActionValue& Value);

	void AdminPanelToggle(const FInputActionValue& Value);

	//Input Values
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputMappingContext* CameraControlsMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* CameraZoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* CameraMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* CameraRotate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* ToggleAdminPanel;

	//Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* CameraArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* cameraMover;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	/** Camera Zoom Properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MinZoomLength = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxZoomLength = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float DefaultArmLength = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomStep = 10.0f;

protected:
	virtual void BeginPlay() override;

private:
	APlayerController* PC;

	AFYPTrainingGameMode* gmRef;
};
