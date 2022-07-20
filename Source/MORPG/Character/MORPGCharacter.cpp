// Copyright Epic Games, Inc. All Rights Reserved.

#include "MORPGCharacter.h"

#include "KismetAnimationLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AMORPGCharacter::AMORPGCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraArm->TargetArmLength = 1200.f;
	CameraArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraArm->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CharacterCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CharacterCameraComponent->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	CharacterCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Navigator
	Navigation = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("Navigator"));
	Navigation->SetGenerationRadii(2000.0f, 2000.0f);
	
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMORPGCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	CurrentCharSpeed = GetVelocity().Size();
	MeshDirection = UKismetAnimationLibrary::CalculateDirection(GetVelocity(),GetActorRotation());
	
	//CalculateAllowSprint();
	
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisX);
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisY);

	//Character rotation to Cursor
	APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (myController){
		if (AxisX!=0 || AxisY!=0){
			FHitResult ResultHit;
			myController->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, ResultHit);
			float FindRotaterResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
			SetActorRotation(FQuat(FRotator(0.0f, FindRotaterResultYaw, 0.0f)));
		}
	}
}

void AMORPGCharacter::InputCameraIn()
{
	if(CameraArm->TargetArmLength <= 300){}
	else
		CameraArm->TargetArmLength -= 75.0f;
}

void AMORPGCharacter::InputCameraOut()
{
	if(CameraArm->TargetArmLength >= 2100){}
	else
		CameraArm->TargetArmLength += 75.0f;
}

void AMORPGCharacter::RotateCamera()
{	
	if(!bStartRotate)
	{
		UE_LOG(LogViewport, Display, TEXT("Command to SWITCH ROTATE CAMERA"));
		bStartRotate = true;
		CameraArm->bUsePawnControlRotation = true;
	}
	else
	{
		UE_LOG(LogViewport, Display, TEXT("Command to STOP SWITCH ROTATE CAMERA"));
		bStartRotate = false;
		CameraArm->bUsePawnControlRotation = false;
		CameraArm->bInheritPitch = true;
		CameraArm->bInheritYaw = true;
	}
}

void AMORPGCharacter::SetupPlayerInputComponent(UInputComponent* NewInput)
{
	Super::SetupPlayerInputComponent(NewInput);
	NewInput->BindAxis(TEXT("MoveForward"), this, &AMORPGCharacter::InputAxisX);
	NewInput->BindAxis(TEXT("MoveRight"), this, &AMORPGCharacter::InputAxisY);
	NewInput->BindAction(TEXT("CameraZoomIn"), IE_Pressed, this, &AMORPGCharacter::InputCameraIn);
	NewInput->BindAction(TEXT("CameraZoomOut"),IE_Pressed, this, &AMORPGCharacter::InputCameraOut);
	NewInput->BindAction(TEXT("CameraRotate"),IE_Pressed, this, &AMORPGCharacter::RotateCamera);
	NewInput->BindAction(TEXT("CameraRotate"),IE_Released, this, &AMORPGCharacter::RotateCamera);
}

void AMORPGCharacter::InputAxisX(float Value)
{
	AxisX = Value;
}

void AMORPGCharacter::InputAxisY(float Value)
{
	AxisY = Value;
}