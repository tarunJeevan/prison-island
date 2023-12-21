#pragma once

#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UInteractionWidget;
class UInventoryWidget;
class UInventoryGridWidget;
struct FInteractableData;

UCLASS()
class PRISONISLAND_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	APlayerHUD();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInventoryGridWidget> InventoryGridWidgetClass;

	bool bIsMenuVisible; // Necessary?

#pragma region Functions
	void OpenInventory() const;
	void CloseInventory() const;

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;
#pragma endregion

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UInventoryGridWidget> InventoryGridWidget;
	
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;

	UPROPERTY()
	TObjectPtr<UInteractionWidget> InteractionWidget;
};
