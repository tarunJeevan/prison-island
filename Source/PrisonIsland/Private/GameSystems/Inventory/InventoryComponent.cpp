#include "GameSystems/Inventory/InventoryComponent.h"
#include "Items/ItemObjectBase.h"
#include "GameSystems/Inventory/InventoryStructs.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Items.SetNum((Columns * Rows));
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsDirty)
	{
		IsDirty = false;
		OnInventoryChanged.Broadcast();
	}
}

auto UInventoryComponent::GetItemAtIndex(int32 Index)
{
	struct result { bool Valid; UItemObjectBase* ItemObject; };

	if (Items.IsValidIndex(Index))
		return result{true, Items[Index]};
	else
		return result{false, nullptr};
}

void UInventoryComponent::AddItemAt(UItemObjectBase* ItemObject, int32 TopLeftIndex)
{
	FTile Tile = IndexToTile(TopLeftIndex);
	FIntPoint Dimensions = ItemObject->GetDimensions();

	int32 LastIndex1 = Tile.X + (Dimensions.X - 1);
	int32 LastIndex2 = Tile.Y + (Dimensions.Y - 1);

	for (int32 i = Tile.X; i < LastIndex1; i++)
	{
		for (int32 j = Tile.Y; j < LastIndex2; j++)
		{
			int32 InIndex = TileToIndex(Tile);
			Items.Insert(ItemObject, InIndex);
		}
	}
	IsDirty = true;
}

bool UInventoryComponent::TryAddItem(UItemObjectBase* ItemObject)
{
	if (ItemObject)
	{
		for (int32 i = 0; i < Items.Num(); i++) {
			if (IsRoomAvailable(ItemObject, i))
			{
				AddItemAt(ItemObject, i);
				return true;
			}
		}
		// Rotate the item and try to add it again
		ItemObject->Rotate();
		for (int32 i = 0; i < Items.Num(); i++) {
			if (IsRoomAvailable(ItemObject, i))
			{
				AddItemAt(ItemObject, i);
				return true;
			}
		}
		// Rotate the item back
		ItemObject->Rotate();
		return false;
	}
	return false;
}

bool UInventoryComponent::IsRoomAvailable(UItemObjectBase* ItemObject, int32 TopLeftIndex)
{
	FTile Tile = IndexToTile(TopLeftIndex);
	FIntPoint Dimensions = ItemObject->GetDimensions();
	
	int32 LastIndex1 = Tile.X + (Dimensions.X - 1);
	int32 LastIndex2 = Tile.Y + (Dimensions.Y - 1);

	for (int32 i = Tile.X; i < LastIndex1; i++)
	{
		for (int32 j = Tile.Y; j < LastIndex2; j++)
		{
			if (!IsTileValid(i, j))
				return false;
			else
			{
				int32 Index = TileToIndex(FTile{ i, j });
				auto result = GetItemAtIndex(Index);

				if (!result.Valid)
					return false;
				else
				{
					if (result.ItemObject)
						return false;
				}
			}
		}
	}
	// Return true only if all checks are passed
	return true;
}

TMap<UItemObjectBase*, FTile> UInventoryComponent::GetAllItems()
{
	TMap<UItemObjectBase*, FTile> AllItems;

	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i])
		{
			if (!AllItems.Contains(Items[i]))
				AllItems.Add(Items[i], IndexToTile(i));
		}
	}

	return AllItems;
}

void UInventoryComponent::RemoveItem(UItemObjectBase* ItemObject)
{
	if (ItemObject)
	{
		for (int32 i = 0; i < Items.Num(); i++)
		{
			if (Items[i] == ItemObject)
			{
				Items[i] = nullptr;
				IsDirty = true;
			}
		}
	}
}

bool UInventoryComponent::IsTileValid(int32 TileX, int32 TileY)
{
	if (TileX >= 0 && TileY >= 0 && TileX < Columns && TileY < Rows)
		return true;

	return false;
}

FTile UInventoryComponent::IndexToTile(int32 Index) const
{
	FTile Tile;
	
	Tile.X = Index % Columns;
	Tile.Y = Index / Columns;

	return Tile;
}

int32 UInventoryComponent::TileToIndex(FTile Tile) const
{
	return Tile.X + (Tile.Y * Columns);
}

