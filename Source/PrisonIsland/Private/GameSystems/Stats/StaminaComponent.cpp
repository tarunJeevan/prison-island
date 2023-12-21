#include "GameSystems/Stats/StaminaComponent.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// Change tick interval as needed so stamina updating looks smooth
	SetComponentTickInterval(0.2f);
	SetComponentTickEnabled(true);

	//bEditableWhenInherited = true;
}

// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	MyOwner = Cast<APlayerCharacter>(GetOwner());
	CurrentStamina = MaxStamina;
}

// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#pragma region Update Stamina
	// Normal stamina recuperation rate is 1 per tick
	int32 ActualStaminaRecuperationFactor = StaminaRecuperationFactor;

	// If player has run since the last tick, decrease stamina in accordance with sprint cost
	if (bHasRan)
		ActualStaminaRecuperationFactor = -RunStaminaCost;
	// If player dashed in the last tick, decrease stamina in accordance with dash cost
	else if (bHasDashed)
		ActualStaminaRecuperationFactor = -DashStaminaCost;
	// If player has crouched since the last tick, increase the stamina recuperation rate
	else if (MyOwner->bIsCrouched)
		ActualStaminaRecuperationFactor = RestStaminaRebate;

	// Save previous stamina value
	const int32 PreviousStamina = CurrentStamina;

	// Update CurrentStamina
	CurrentStamina = FMath::Clamp(CurrentStamina + ActualStaminaRecuperationFactor, 0, MaxStamina);

	// If CurrentStamina was changed, broadcast the stamina changed event
	if (CurrentStamina != PreviousStamina)
	{
		OnStaminaChanged.Broadcast( CurrentStamina, MaxStamina);
	}

	// bHasRan will be set to true in another function if the player is sprinting, so it needs to be set to false here
	bHasRan = false;
	bHasDashed = false;
#pragma endregion
}

#pragma region Getters and Setters
// Get current stamina
int32 UStaminaComponent::GetStamina()
{
	return CurrentStamina;
}

// Get normal stamina recuperation factor
int32 UStaminaComponent::GetStaminaRecuperationFactor()
{
	return StaminaRecuperationFactor;
}

// Set normal stamina recuperation factor
void UStaminaComponent::SetStaminaRecuperationFactor(int32 NewFactor)
{
	StaminaRecuperationFactor = NewFactor;
}

// Get max stamina
int32 UStaminaComponent::GetMaxStamina()
{
	return MaxStamina;
}

// Get the value of bIsRunning
bool UStaminaComponent::GetIsRunning()
{
	return bIsRunning;
}

// Set dashing cost
void UStaminaComponent::SetDashStaminaCost(int32 NewCost)
{
	DashStaminaCost = NewCost;
}

// Get dashing stamina cost
int32 UStaminaComponent::GetDashStaminaCost()
{
	return DashStaminaCost;
}

// Get running stamina cost
int32 UStaminaComponent::GetRunStaminaCost()
{
	return RunStaminaCost;
}

// Set running cost
void UStaminaComponent::SetRunStaminaCost(int32 NewCost)
{
	RunStaminaCost = NewCost;
}

// Set boosted stamina recovery rate
void UStaminaComponent::SetStaminaRebate(int32 NewRebate)
{
	RestStaminaRebate = NewRebate;
}

// Modify running state to control character movement speed
void UStaminaComponent::SetRunning(bool bNewValue)
{
	bIsRunning = bNewValue;

	if (MyOwner)
		MyOwner->GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? RunningMaxWalkSpeed : NormalMaxWalkSpeed;
}

// Sets bHasRan, which controls stamina drain for running
void UStaminaComponent::SetHasRan(bool bNewValue)
{
	bHasRan = bNewValue;
}
#pragma endregion
