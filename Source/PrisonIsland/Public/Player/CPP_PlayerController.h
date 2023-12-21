// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/PlayerController.h"
#include "CPP_PlayerController.generated.h"

#pragma region Forward Declarations

class APlayerCharacter;
class UCameraComponent;
class USpringArmComponent;
class UCharacterMovementComponent;
class UStaminaComponent;
class UCurveFloat;
class UInputAction;
class UInputMappingContext;
class UInputComponent;
class UEnhancedInputLocalPlayerSubsystem;

#pragma endregion

UCLASS(Abstract)
class PRISONISLAND_API ACPP_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACPP_PlayerController();
	
private:
	bool bIsRunning = false;
	bool bFlipFlopVar;

protected:
#pragma region Player Character Properties
	UPROPERTY(BlueprintReadOnly, Category = "Properties|Perspective")
	bool bInFPP = false;

	UPROPERTY(BlueprintReadWrite, Category = "Properties|Dashing")
	bool bCanDash = true;

	UPROPERTY(BlueprintReadOnly, Category = "Properties|Turning")
	float TurnRate;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Properties|Perspective")
	float ZoomIncrement;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Properties|Dashing")
	float DashDistance;

	UPROPERTY(EditAnywhere, Category = "Properties|Dashing")
	TObjectPtr<UCurveFloat> DashCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|Speed")
	float NormalMaxWalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|Speed")
	float RunningMaxWalkSpeed = 500.0f;
#pragma endregion

#pragma region Input Actions
	// Input Mapping Context for standard use
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Input Mapping Context")
	TObjectPtr<UInputMappingContext> PlayerStandardContext = nullptr;
	
	// Input Mapping Context for combat use
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Input Mapping Context")
	TObjectPtr<UInputMappingContext> PlayerCombatContext = nullptr;

	// Input Action for normal movement (walking)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Standard Input Actions")
	TObjectPtr<UInputAction> MoveAction = nullptr;

	// Input Action for looking around
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Standard Input Actions")
	TObjectPtr<UInputAction> LookAction = nullptr;

	// Input Action for jumping
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Standard Input Actions")
	TObjectPtr<UInputAction> JumpAction = nullptr;

	// Input Action for crouching
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Standard Input Actions")
	TObjectPtr<UInputAction> CrouchAction = nullptr;

	// Input Action for allowing sprinting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Standard Input Actions")
	TObjectPtr<UInputAction> SprintAction = nullptr;

	// Input Action for dashing in a specific direction
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Standard Input Actions")
	TObjectPtr<UInputAction> DashAction = nullptr;

	// Input Action for interacting with the environment
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Standard Input Actions")
	TObjectPtr<UInputAction> InteractAction = nullptr;

	// Input Action for opening and closing player inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Standard Input Actions")
	TObjectPtr<UInputAction> ToggleBackpackAction = nullptr;

	// Input Action for zooming in and out and switching player modes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Standard Input Actions")
	TObjectPtr<UInputAction> CameraZoomAction = nullptr;

	// Input Action for punching (used with the combat IMC)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Combat Input Actions")
	TObjectPtr<UInputAction> PunchAction = nullptr;

	// Input Action for blocking (used with the combat IMC)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Combat Input Actions")
	TObjectPtr<UInputAction> BlockAction = nullptr;

	// Input Action for kicking (used with the combat IMC)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Combat Input Actions")
	TObjectPtr<UInputAction> KickAction = nullptr;

#pragma endregion

#pragma region Controller Attributes
	// Reference to possessed player character
	UPROPERTY()
	TObjectPtr<APlayerCharacter> PossessedPlayer = nullptr;

	// Reference to Enhanced Input Component that handles input
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = nullptr;

	// Reference to possessed player's character movement component
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> PlayerMoveComponent = nullptr;

	// Reference to possessed player's stamina component
	UPROPERTY()
	TObjectPtr<UStaminaComponent> PlayerStaminaComponent = nullptr;

	// Reference to Enhanced Input's local player subsystem to handle player input
	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = nullptr;

	// Reference to possessed player's FPP Camera
	UPROPERTY()
	TObjectPtr<UCameraComponent> PlayerFPPCam = nullptr;

	// Reference to possessed player's TPP Camera
	UPROPERTY()
	TObjectPtr<UCameraComponent> PlayerTPPCam = nullptr;

	// Reference to possessed player's TPP camera spring arm
	UPROPERTY()
	TObjectPtr<USpringArmComponent> TPPCameraBoom = nullptr;

	// Reference to possessed player's FPP camera spring arm
	UPROPERTY()
	TObjectPtr<USpringArmComponent> FPPCameraBoom = nullptr;

	// The location to dash
	UPROPERTY()
	FVector DashLocation;

	// Dash speed
	UPROPERTY(BlueprintReadWrite, Category = "Properties|Dashing")
	FVector DashVelocity;
#pragma endregion

#pragma region Timelines

	UPROPERTY()
	FTimeline DashTimeline;

#pragma endregion
	
#pragma region Overrides
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
#pragma endregion

private:
#pragma region Input Functions

	// Handles player movement input
	void Move(const FInputActionValue& Value);
	// Handles player look input
	void Look(const FInputActionValue& Value);
	// Handles Jumping
	void PlayerJump();
	// Handles jump ending
	void PlayerEndJump();
	// Handles player crouching
	void PlayerCrouch();
	// Handles player sprinting
	void PlayerSprint();
	// Resets player walk speed to normal
	void PlayerSprintEnd();
	// Handles camera zoom and switches from FPP to TPP and vice versa
	void PlayerCameraZoom(const FInputActionValue& Value);
	
	// Handles player interaction
	void PlayerInteract();
	// Handles player interaction end
	void PlayerEndInteract();
	// Handles opening and closing inventory
	void ToggleBackpack();

	// Handles player punching
	void PlayerPunch();
	// Handles player kicking
	void PlayerKick();
	// Handles player blocking
	void PlayerBlock();
	// Handles player dashing
	void PlayerDash();

#pragma endregion

#pragma region Helper Functions
	UFUNCTION()
	void DashUpdate(float Alpha);

	UFUNCTION()
	void DashFinished();

	UFUNCTION()
	void SetCanDash(bool NewValue);
#pragma endregion
};
