#pragma once

#include "GameFramework/Character.h"
#include "Interfaces/InteractionInterface.h"
#include "PlayerCharacter.generated.h"

class UStaminaComponent;
class UHealthComponent;
class UInventoryComponent;
class UDamageType;
class AController;
class APlayerHUD;

USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()

	FInteractionData() : 
		CurrentInteractable(nullptr),
		LastInteractionCheckTime(0.0f)
	{};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS()
class PRISONISLAND_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(InteractionTimerHandler); };
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
		
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TObjectPtr<APlayerHUD> HUD;

#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> HealthComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaminaComponent> StaminaComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> InventoryComponent = nullptr;
#pragma endregion

#pragma region Interaction
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UPROPERTY(EditDefaultsOnly)
	float InteractionCheckDistance;

	UPROPERTY(EditDefaultsOnly)
	float InteractionCheckFrequency;

	FTimerHandle InteractionTimerHandler;

	FInteractionData InteractionData;

	// Functions
	void CastInteractionTrace();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
public:
	void BeginInteract();
	void EndInteract();
	void Interact();
#pragma endregion
};
