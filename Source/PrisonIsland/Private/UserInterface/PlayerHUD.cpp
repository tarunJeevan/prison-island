#include "UserInterface/PlayerHUD.h"
#include "UserInterface/Interaction/InteractionWidget.h"
#include "UserInterface/Inventory/InventoryWidget.h"
#include "UserInterface/Inventory/InventoryGridWidget.h"

APlayerHUD::APlayerHUD()
{

}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget->AddToViewport(4);
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InventoryGridWidgetClass)
	{
		InventoryGridWidget = CreateWidget<UInventoryGridWidget>(GetWorld(), InventoryGridWidgetClass);
		InventoryGridWidget->AddToViewport(5);
		InventoryGridWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerHUD::OpenInventory() const
{
	if (InventoryWidget)
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
}

void APlayerHUD::CloseInventory() const
{
	if (InventoryWidget)
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void APlayerHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
}

void APlayerHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void APlayerHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);

		InteractionWidget->UpdateWidget(InteractableData);
	}
}
