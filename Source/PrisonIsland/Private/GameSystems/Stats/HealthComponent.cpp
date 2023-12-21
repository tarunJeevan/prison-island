#include "GameSystems/Stats/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	// Get owner of this component
	AActor* Owner = GetOwner();

	if (Owner)
	{
		// Bind HandleDamage function to owner's OnTakeAnyDamage event
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleDamage);
	}
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Called whenever owner's OnTakeAnyDamage event is fired
void UHealthComponent::HandleDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// Do nothing if damage is 0
	if (Damage <= 0.0f) return;

	// Do nothing if health is already depleted
	if (CurrentHealth <= 0.0f) return;

	// Save current health value
	float PreviousHealth = CurrentHealth;

	// Reduce health by damage amount
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	if (CurrentHealth != PreviousHealth)
	{
	// Broadcast change in health so player and/or HUD can update accordingly
	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
	}

	// Broadcast health depletion so player character can handle player death
	if (CurrentHealth <= 0.0f)
	{
		OnPlayerDeath.Broadcast();
	}
}

// Return maximum health
float UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}

// Set maximum health
void UHealthComponent::SetMaxHealth(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
}

// Return current health
float UHealthComponent::GetHealth()
{
	return CurrentHealth;
}

// Restore current health to max health
void UHealthComponent::RestoreToFullHealth()
{
	CurrentHealth = MaxHealth;
}

