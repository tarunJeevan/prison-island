// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Data/ItemDataStruct.h"
#include "../Player/PlayerCharacter.h"
#include "ItemBase.generated.h"

UCLASS()
class PRISONISLAND_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
#pragma region Properties
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemStatistics ItemStatistics;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemAssetData AssetData;
#pragma endregion

#pragma region Functions
	// Constructor
	UItemBase();

	TObjectPtr<UItemBase> CreateItemCopy() const;

	virtual void Use(APlayerCharacter* Character);

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemWeight() const { return ItemStatistics.Weight; };
#pragma endregion

protected:
	// Used to compare items in the inventory component
	bool operator==(const FName& OtherID) const {
		return this->ID == OtherID;
	}
};
