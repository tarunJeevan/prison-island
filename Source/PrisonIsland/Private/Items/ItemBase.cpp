// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"

UItemBase::UItemBase()
{
}

TObjectPtr<UItemBase> UItemBase::CreateItemCopy() const
{
	// Create new item object
	TObjectPtr<UItemBase> ItemCopy = NewObject<UItemBase>(StaticClass());

	// Copy current item data to copy
	ItemCopy->ID = this->ID;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->TextData = this->TextData;
	ItemCopy->AssetData = this->AssetData;

	// Return pointer to new item object
	return ItemCopy;
}

void UItemBase::Use(APlayerCharacter* Character)
{
}
