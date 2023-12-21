// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryStructs.generated.h"

UENUM()
enum class EPickupItemType : uint8
{
	Medicinal UMETA(DisplayName = "Medicinal"),
	CraftingPart UMETA(DisplayName = "CraftingPart"),
	Ammo UMETA(DisplayName = "Ammo"),
	Throwable UMETA(DisplayName = "Throwable"),
	Equipment UMETA(DisplayName = "Equipmemt"),
	Quest UMETA(DisplayName = "Quest")
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY()

	FItemData() :
		ItemType(EPickupItemType::Equipment),
		Name(FText::GetEmpty()),
		Description(FText::GetEmpty()),
		Durability(0.0f),
		Weight(0.0f),
		DamageValue(0.0f),
		RestorationValue(0.0f)
	{};

	UPROPERTY(EditInstanceOnly)
	EPickupItemType ItemType;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Description;

	UPROPERTY(EditInstanceOnly)
	float Durability;

	UPROPERTY(EditInstanceOnly)
	float Weight;

	UPROPERTY(EditInstanceOnly)
	float DamageValue;

	UPROPERTY(EditInstanceOnly)
	float RestorationValue;
};

USTRUCT(BlueprintType)
struct FLine
{
	GENERATED_USTRUCT_BODY()

	// Move code to inventory component if it's the only thing that needs this struct
	UPROPERTY(BlueprintReadWrite)
	FVector2D Start;

	UPROPERTY(BlueprintReadWrite)
	FVector2D End;
};

USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 X;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Y;
};