#pragma once

#include "UObject/Object.h"
#include "Interfaces/InteractionInterface.h"
#include "GameSystems/Inventory/InventoryStructs.h"
#include "ItemObjectBase.generated.h"

class AItemActorBase;

UCLASS(Blueprintable)
class PRISONISLAND_API UItemObjectBase : public UObject
{
	GENERATED_BODY()
	
public:
#pragma region Functions
	UItemObjectBase();

	FORCEINLINE FInteractableData GetInteractionData() const { return InteractionData; };
	FORCEINLINE FItemData GetItemData() const { return ItemData; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FIntPoint GetDimensions() const { return Rotated ? FIntPoint{ Dimensions.Y, Dimensions.X } : Dimensions; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UMaterialInterface* GetIcon() const { return Rotated ? IconRotated : Icon; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TSubclassOf<AItemActorBase> GetItemClass() const { return ItemClass; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsRotated() const { return Rotated; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void Rotate() { Rotated = !Rotated; };
#pragma endregion

protected:
#pragma region Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn="true"))
	FIntPoint Dimensions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UMaterialInterface> Icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UMaterialInterface> IconRotated;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSubclassOf<AItemActorBase> ItemClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FInteractableData InteractionData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FItemData ItemData;

	UPROPERTY()
	bool Rotated;
#pragma endregion
};
