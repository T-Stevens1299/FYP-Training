// Copyright Stevens Studios Ltd.
// Written By Thomas Stevens

#include "RTS_Camera.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h" 
#include <Kismet/GameplayStatics.h>
#include "InputActionValue.h"

// Sets default values
ARTS_Camera::ARTS_Camera()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
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
}

void ARTS_Camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		const FRotator Rotation = Controller->GetControlRotation();
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