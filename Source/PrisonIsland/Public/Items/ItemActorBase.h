#pragma once

#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "GameSystems/Inventory/InventoryStructs.h"
#include "ItemActorBase.generated.h"

class UItemObjectBase;

UCLASS()
class PRISONISLAND_API AItemActorBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActorBase();

protected:
#pragma region Properties
	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item Data")
	FInteractableData InstanceInteractableData;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UItemObjectBase> ItemObject;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item Data")
	FItemData InstanceItemData;
#pragma endregion

#pragma region Functions
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (ToolTip = "Create a default Item Object"))
	UItemObjectBase* GetDefaultItemObject();

	// Interface overrides
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract_Implementation() override;
	virtual void EndInteract_Implementation() override;
	virtual void Interact_Implementation(APlayerCharacter* Character) override;
#pragma endregion
};
