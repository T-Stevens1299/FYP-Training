// Written by Thomas Stevens, all rights reserved

#include "RTS_Camera.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h" 
#include <Kismet/GameplayStatics.h>
#include "InputActionValue.h"
#include "FYPTraining/FYPTrainingGameMode.h"

// Sets default values
ARTS_Camera::ARTS_Camera()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	cameraMover = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	cameraMover->SetupAttachment(GetRootComponent());

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(cameraMover);
	CameraArm->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f)); // Position the camera
	CameraArm->TargetArmLength = 400.0f;
	CameraArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void ARTS_Camera::BeginPlay()
{
	Super::BeginPlay();
	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PC->bShowMouseCursor = true;

	AFYPTrainingGameMode* gamemode = Cast<AFYPTrainingGameMode>(UGameplayStatics::GetGameMode(this));
	if (gamemode) { gmRef = gamemode; }
}

void ARTS_Camera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CameraControlsMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(CameraZoom, ETriggerEvent::Triggered, this, &ARTS_Camera::ZoomCamera);

		EnhancedInputComponent->BindAction(CameraMove, ETriggerEvent::Triggered, this, &ARTS_Camera::MoveCamera);

		EnhancedInputComponent->BindAction(CameraRotate, ETriggerEvent::Triggered, this, &ARTS_Camera::RotateCamera);

		EnhancedInputComponent->BindAction(ToggleAdminPanel, ETriggerEvent::Triggered, this, &ARTS_Camera::AdminPanelToggle);
	}
}

void ARTS_Camera::ZoomCamera(const FInputActionValue& Value)
{
	const float newTargetArmLength = CameraArm->TargetArmLength + (Value.Get<float>() * ZoomStep);

	CameraArm->TargetArmLength = FMath::Clamp(newTargetArmLength, MinZoomLength, MaxZoomLength);
}

void ARTS_Camera::MoveCamera(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = CameraArm->GetRelativeRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARTS_Camera::RotateCamera(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();

	if (Controller != nullptr)
	{
		CameraArm->AddRelativeRotation(FRotator(0.0f, AxisValue, 0.0f));
	}
}

void ARTS_Camera::AdminPanelToggle(const FInputActionValue& Value)
{
	gmRef->ToggleAdminPanel();
}