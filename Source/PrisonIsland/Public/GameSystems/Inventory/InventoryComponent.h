#pragma once

#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

class UItemObjectBase;
struct FTile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRISONISLAND_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable)
	FOnInventoryChanged OnInventoryChanged;

	UInventoryComponent();

	FORCEINLINE int32 GetColumns() const { return Columns; };
	FORCEINLINE int32 GetRows() const { return Rows; };

	UFUNCTION(BlueprintCallable)
	bool TryAddItem(UItemObjectBase* ItemObject);

	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable(UItemObjectBase* ItemObject, int32 TopLeftIndex);

	UFUNCTION(BlueprintCallable)
	TMap<UItemObjectBase*, FTile> GetAllItems();

	UFUNCTION(BlueprintCallable)
	void RemoveItem(UItemObjectBase* ItemObject);

	UFUNCTION(BlueprintCallable)
	void AddItemAt(UItemObjectBase* ItemObject, int32 TopLeftIndex);

	UFUNCTION(BlueprintCallable)
	int32 TileToIndex(FTile Tile) const;

	bool IsTileValid(int32 TileX, int32 TileY);

	FTile IndexToTile(int32 Index) const;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants", meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 Columns;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants", meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 Rows;

	TArray<UItemObjectBase*> Items;
	
	bool IsDirty;

	auto GetItemAtIndex(int32 Index);

};
