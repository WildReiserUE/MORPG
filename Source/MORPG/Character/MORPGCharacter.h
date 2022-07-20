// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NavigationInvokerComponent.h"
#include "GameFramework/Character.h"
#include "MORPGCharacter.generated.h"

UCLASS(Blueprintable)
class AMORPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMORPGCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCharacterCameraComponent() const { return CharacterCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraArm() const { return CameraArm; }

private:
	virtual void SetupPlayerInputComponent(class UInputComponent* Input) override;
	void InputAxisX(float Value);
	void InputAxisY(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CharacterCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Naviagation, meta = (AllowPrivateAccess = "true"))
	UNavigationInvokerComponent* Navigation;
	
	UFUNCTION()
		void InputCameraIn();
	UFUNCTION()
		void InputCameraOut();
	void RotateCamera();

	bool bStartRotate = false;
	float AxisX = 0.0f;
	float AxisY = 0.0f;
	float CurrentCharSpeed = 0.0f;
	float MeshDirection = 0.0f;
};

