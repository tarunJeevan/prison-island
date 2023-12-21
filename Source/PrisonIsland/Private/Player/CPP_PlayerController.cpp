#include "Player/CPP_PlayerController.h"

#include "Player/PlayerCharacter.h"
#include "Curves/CurveFloat.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameSystems/Stats/HealthComponent.h"
#include "GameSystems/Stats/StaminaComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ACPP_PlayerController::ACPP_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	bFlipFlopVar = true;

	DashDistance = 500.0f;
	ZoomIncrement = 30.0f;
}

void ACPP_PlayerController::PlayerCameraZoom(const FInputActionValue& Value)
{
	float ZoomValue = Value.Get<float>(); // Value is 1 for scroll up and -1 for scroll down

	if (PlayerFPPCam->IsActive()) // Player is in FPP
	{
		if (ZoomValue < 0) // Player wants to zoom out
		{
			// Switch to third person perspective
			PlayerTPPCam->Activate();
			PlayerFPPCam->Deactivate();
			PossessedPlayer->bUseControllerRotationYaw = false;
			bInFPP = false;

			TPPCameraBoom->TargetArmLength = 50.0f;

			// Remove combat input context when not in FPP
			Subsystem->RemoveMappingContext(PlayerCombatContext);
		}
	}
	else // Player is in TPP
	{
		if (ZoomValue < 0) // Player wants to zoom out
		{
			TPPCameraBoom->TargetArmLength = UKismetMathLibrary::FClamp((TPPCameraBoom->TargetArmLength - (ZoomValue * ZoomIncrement)), 0, 500);
		}
		else // Player wants to zoom in
		{
			TPPCameraBoom->TargetArmLength = UKismetMathLibrary::FClamp((TPPCameraBoom->TargetArmLength - (ZoomValue * ZoomIncrement)), 0, 500);
		}

		if (TPPCameraBoom->TargetArmLength < 50.0f) // If the TPPCamera is close enough, switch to FPPCamera
		{
			PlayerFPPCam->Activate();
			PlayerTPPCam->Deactivate();
			PossessedPlayer->bUseControllerRotationYaw = true;
			bInFPP = true;
			PlayerStaminaComponent->SetRunning(false); // Stop sprinting if it was on

			// Add combat input context when in FPP
			Subsystem->AddMappingContext(PlayerCombatContext, 1);
		}
	}
}

void ACPP_PlayerController::DashUpdate(float Alpha)
{
	PossessedPlayer->SetActorLocation(UKismetMathLibrary::VLerp(PossessedPlayer->GetActorLocation(), DashLocation, Alpha) );
}

void ACPP_PlayerController::DashFinished()
{
	PlayerMoveComponent->Velocity = DashVelocity;
}

#pragma region Input Handlers
void ACPP_PlayerController::Move(const FInputActionValue& Value)
{
	// Add checks here to see if player is allowed to move around

	if (PossessedPlayer)
	{
		// Initialize movement vector
		const FVector2D MovementVec = Value.Get<FVector2D>();
		
		// Get controller's forward direction
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

		// Add forward movement to Character along the Controller's forward direction
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		PossessedPlayer->AddMovementInput(ForwardDirection, MovementVec.Y);

		// Add lateral movement to Character along the Controller's Right Vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		PossessedPlayer->AddMovementInput(RightDirection, MovementVec.X);

		if (PlayerStaminaComponent->GetIsRunning())
			PlayerStaminaComponent->SetHasRan(true);
	}
}

void ACPP_PlayerController::Look(const FInputActionValue& Value)
{
	// Add checks here to see if player is allowed to look around

	// Initialize look axis vector
	const FVector2D LookVector = Value.Get<FVector2D>();
	TurnRate = LookVector.X;

	// Handles turning left and right
	AddYawInput(LookVector.X);

	// Handles turning up and down
	AddPitchInput(LookVector.Y);
}

void ACPP_PlayerController::PlayerJump()
{
	// Add checks here to see if player is allowed to jump

	// Handle character jump. Uncrouch if crouched
	if (PossessedPlayer) {
		PossessedPlayer->UnCrouch();
		PossessedPlayer->Jump();
	}
}

void ACPP_PlayerController::PlayerEndJump()
{
	if (PossessedPlayer)
		PossessedPlayer->StopJumping();
}

void ACPP_PlayerController::PlayerCrouch()
{
	// Check if player character can crouch

	if (PossessedPlayer && PossessedPlayer->bIsCrouched) // If player is already crouched, then Uncrouch(), else Crouch().
		PossessedPlayer->UnCrouch();
	else
	{
		PlayerStaminaComponent->SetRunning(false);
		PossessedPlayer->Crouch();
	}
}

void ACPP_PlayerController::PlayerSprint()
{
	// Check current stamina and only allow running if stamina is greater than running stamina cost
	if (PlayerStaminaComponent->GetStamina() >= PlayerStaminaComponent->GetRunStaminaCost())
		PlayerStaminaComponent->SetRunning(true);
	else
		PlayerStaminaComponent->SetRunning(false);
}

void ACPP_PlayerController::PlayerSprintEnd()
{
	// Disable running stamina drain logic and reset character movement speed to walking
	PlayerStaminaComponent->SetRunning(false);
}

void ACPP_PlayerController::SetCanDash(bool NewValue)
{
	bCanDash = NewValue;
}

void ACPP_PlayerController::PlayerDash()
{
	// Play dash animation
	FVector LastInputVec = PlayerMoveComponent->GetLastInputVector();	

	if (!PlayerMoveComponent->IsFalling()) // Add check for crouching. PossessedPlayer->bIsCrouched not working.
	{
		if (bCanDash)
		{
			bCanDash = false;
			if (UAIBlueprintHelperLibrary::IsValidAIDirection(LastInputVec))
			{
				// Prepare for line trace
				FVector TraceStart = PossessedPlayer->GetActorLocation();
				FVector TraceEnd = TraceStart + (LastInputVec * DashDistance);
				FCollisionQueryParams TraceParams;
				TraceParams.AddIgnoredActor(this); // Add PossessedPlayer to ActorsToIgnore?
				FHitResult HitResult;

				if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, TraceParams))
				{		
					DashLocation = (HitResult.Location + (LastInputVec * -45.0f));
					DashVelocity = LastInputVec;

					DashTimeline.PlayFromStart();

					// Delay for two seconds, then set bCanDash to true
					FTimerHandle TimerHandle;
					FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ACPP_PlayerController::SetCanDash, true);
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 2, false);
				}
				else
				{
					DashLocation = HitResult.TraceEnd;
					DashVelocity = LastInputVec;

					DashTimeline.PlayFromStart();

					// Delay for two seconds, then set bCanDash to true
					FTimerHandle TimerHandle;
					FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ACPP_PlayerController::SetCanDash, true);
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 2, false);
				}
			}
			else
			{
				// Prepare for line trace
				FVector TraceStart = PossessedPlayer->GetActorLocation();
				FVector ForwardVec = PossessedPlayer->GetActorForwardVector();
				FVector TraceEnd = TraceStart + (ForwardVec * DashDistance);
				FCollisionQueryParams TraceParams;
				TraceParams.AddIgnoredActor(this); // Add PossessedPlayer to ActorsToIgnore?
				FHitResult HitResult;

				if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, TraceParams))
				{
					DashLocation = (HitResult.Location + (ForwardVec * -45.0f));
					DashVelocity = ForwardVec;

					DashTimeline.PlayFromStart();

					// Delay for two seconds, then set bCanDash to true
					FTimerHandle TimerHandle;
					FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ACPP_PlayerController::SetCanDash, true);
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 2, false);
				}
				else
				{
					DashLocation = HitResult.TraceEnd;
					DashVelocity = ForwardVec;

					DashTimeline.PlayFromStart();

					// Delay for two seconds, then set bCanDash to true
					FTimerHandle TimerHandle;
					FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ACPP_PlayerController::SetCanDash, true);
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 2, false);
				}
			}
		}
	}
}

void ACPP_PlayerController::PlayerInteract()
{
	// Check if player is within interaction actor range and call interact event
	PossessedPlayer->BeginInteract();
}

void ACPP_PlayerController::PlayerEndInteract()
{
	PossessedPlayer->EndInteract();
}

void ACPP_PlayerController::ToggleBackpack()
{
	// Talk to inventory component attached to Character (if they have one)
	// (Option 1) Create inventory widget, then hide and reveal as required
	// (Option 2) Construct and destroy inventory widget as requested
}

void ACPP_PlayerController::PlayerPunch()
{
	// Play punching animation
	// Check if punch hit something and handle damage calculation
}

void ACPP_PlayerController::PlayerKick()
{
	// Play kicking animation 
	// Check if kick hit something and handle damage calculation
}

void ACPP_PlayerController::PlayerBlock()
{
	// Play block animation
	// Check if something hit player and perform damage calculation
}
#pragma endregion

void ACPP_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DashTimeline.TickTimeline(DeltaTime);
}

void ACPP_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// Get reference to possessed Pawn and store it
	PossessedPlayer = Cast<APlayerCharacter>(aPawn);
	checkf(PossessedPlayer, TEXT("ACPP_PlayerController derived classes should only use APlayerCharacter derived pawns"));

	PlayerStaminaComponent = PossessedPlayer->FindComponentByClass<UStaminaComponent>();
	checkf(PlayerStaminaComponent, TEXT("Player character should have a stamina component"));

	TObjectPtr<UHealthComponent> PlayerHealthComponent = PossessedPlayer->FindComponentByClass<UHealthComponent>();

#pragma region References to player character attributes

	PlayerMoveComponent = PossessedPlayer->GetCharacterMovement();
	PlayerFPPCam = Cast<UCameraComponent>(PossessedPlayer->GetComponentsByTag(UCameraComponent::StaticClass(), FName("FPPCamera"))[0]);
	PlayerTPPCam = Cast<UCameraComponent>(PossessedPlayer->GetComponentsByTag(UCameraComponent::StaticClass(), FName("TPPCamera"))[0]);
	TPPCameraBoom = Cast<USpringArmComponent>(PossessedPlayer->GetComponentsByTag(USpringArmComponent::StaticClass(), FName("TPPCameraBoom"))[0]);
	FPPCameraBoom = Cast<USpringArmComponent>(PossessedPlayer->GetComponentsByTag(USpringArmComponent::StaticClass(), FName("FPPCameraBoom"))[0]);
#pragma endregion

#pragma region Dash handling

	// Bind DashTimeline Update and Finished functions
	FOnTimelineFloat DashProgressUpdate;
	FOnTimelineEvent DashFinishedEvent;
	DashProgressUpdate.BindUFunction(this, FName("DashUpdate"));
	DashFinishedEvent.BindUFunction(this, FName("DashFinished"));

	if (DashCurve)
	{
		DashTimeline.AddInterpFloat(DashCurve, DashProgressUpdate);
		DashTimeline.SetTimelineFinishedFunc(DashFinishedEvent);
	}

#pragma endregion

	// Get reference to the EnhancedInputComponent and store it
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EnhancedInputComponent, TEXT("Unable to get a reference to the Enhanced Input Component"));

	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(Subsystem, TEXT("Unable to get a reference to the EnhancedInputLocalPlayerSubsystem"));

	checkf(PlayerStandardContext, TEXT("Standard Input Mapping Context was not specified"));
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(PlayerStandardContext, 0);

#pragma region Bind input actions

	if (MoveAction) 
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_PlayerController::Move);

	if (LookAction)
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPP_PlayerController::Look);

	if (JumpAction) {
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACPP_PlayerController::PlayerJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACPP_PlayerController::PlayerEndJump);
	}

	if (CrouchAction)
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ACPP_PlayerController::PlayerCrouch);

	if (SprintAction) {
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ACPP_PlayerController::PlayerSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACPP_PlayerController::PlayerSprintEnd);
	}

	if (InteractAction) {
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACPP_PlayerController::PlayerInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ACPP_PlayerController::PlayerEndInteract);
	}

	if (DashAction) 
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ACPP_PlayerController::PlayerDash);

	if (CameraZoomAction) 
		EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &ACPP_PlayerController::PlayerCameraZoom);
#pragma endregion
}

void ACPP_PlayerController::OnUnPossess()
{
	// Unbind things
	EnhancedInputComponent->ClearActionBindings();

	Super::OnUnPossess();
}