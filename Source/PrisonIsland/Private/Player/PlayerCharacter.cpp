#include "Player/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameSystems/Stats/HealthComponent.h"
#include "GameSystems/Stats/StaminaComponent.h"
#include "GameSystems/Inventory/InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UserInterface/PlayerHUD.h"
#include "DrawDebugHelpers.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SetActorTickInterval(0.5f);
	SetActorTickEnabled(true);

	GetCapsuleComponent()->InitCapsuleSize(45.0f, 95.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Adding health, stamina, and inventory components
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
	StaminaComponent->RegisterComponent();
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->RegisterComponent();
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->RegisterComponent();

	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 300.0f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 75.0f;

		// Set Mesh collision details here using GetMesh()
		// GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

		GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
		GetCharacterMovement()->BrakingDecelerationWalking = 200.0f;
	}

	HUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
		CastInteractionTrace();
}
#pragma region Interaction
void APlayerCharacter::CastInteractionTrace()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ GetPawnViewLocation() };
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };

	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());
	if (LookDirection > 0.2f)
	{
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
		{
			if (TraceHit.GetActor()->Implements<UInteractionInterface>())
			{
				// Check if the hit actor is new
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}
				// Check if the hit actor is the one found in the previous line trace
				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
					return;
			}
		}
	}
	// Line trace found nothing
	NoInteractableFound();
}

void APlayerCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
		EndInteract();

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();
}

void APlayerCharacter::NoInteractableFound()
{
	if (IsInteracting())
		GetWorldTimerManager().ClearTimer(InteractionTimerHandler);

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
			TargetInteractable->EndFocus();

		HUD->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void APlayerCharacter::BeginInteract()
{
	// Verify nothing has changed with the interactable since beginning interaction
	CastInteractionTrace();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->Execute_BeginInteract(InteractionData.CurrentInteractable);

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
				Interact();
			else
				GetWorldTimerManager().SetTimer(InteractionTimerHandler, 
					this, 
					&APlayerCharacter::Interact, 
					TargetInteractable->InteractableData.InteractionDuration, 
					false);
		}
	}
}

void APlayerCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(InteractionTimerHandler);

	if (IsValid(TargetInteractable.GetObject()))
		TargetInteractable->Execute_EndInteract(InteractionData.CurrentInteractable);
}

void APlayerCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(InteractionTimerHandler);

	if (IsValid(TargetInteractable.GetObject()))
		TargetInteractable->Execute_Interact(InteractionData.CurrentInteractable, this);
}
#pragma endregion