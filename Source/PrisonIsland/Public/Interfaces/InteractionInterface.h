#pragma once

#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UENUM()
enum class EInteractableType : uint8
{
	Pickup UMETA(DisplayName = "Pickup"),
	NPC UMETA(DisplayName = "NPC"),
	Device UMETA(DisplayName = "Device"),
	Toggle UMETA(DisplayName = "Toggle"),
	Container UMETA(DisplayName = "Container")
};

USTRUCT(BlueprintType)
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()

	FInteractableData() :
		InteractableType(EInteractableType::Pickup),
		Name(FText::GetEmpty()),
		Action(FText::GetEmpty()),
		InteractionDuration(0.0f)
	{};

	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class PRISONISLAND_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginFocus();
	virtual void EndFocus();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginInteract();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndInteract();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(class APlayerCharacter* Character);

	FInteractableData InteractableData;
};
