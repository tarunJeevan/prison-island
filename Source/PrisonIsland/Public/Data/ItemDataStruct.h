// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStruct.generated.h"

UENUM()
enum class EItemType : uint8
{
	Consumable UMETA(DisplayName = "Consumable"),
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor"),
	Quest UMETA(DisplayName = "Quest")
};

USTRUCT()
struct FItemStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Durability;

	UPROPERTY(EditAnywhere)
	float Weight;

	UPROPERTY(EditAnywhere)
	float DamageValue;

	UPROPERTY(EditAnywhere)
	float RestorationValue;
};

USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractionText;

	UPROPERTY(EditAnywhere)
	FText UsageText;
};

USTRUCT()
struct FItemDimensions
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (UIMin=1, UIMax=10))
	uint8 Width;

	UPROPERTY(EditAnywhere, meta = (UIMin = 1, UIMax = 10))
	uint8 Height;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Mesh = nullptr;
};

USTRUCT()
struct FItemDataStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemDimensions ItemDimensions;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;
};
