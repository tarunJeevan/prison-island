#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaDepletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaUpdatedSignature,
											int32, CurrentStamina,
											int32, MaxStamina);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRISONISLAND_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Delegates
	UPROPERTY(BlueprintAssignable)
	FOnStaminaUpdatedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaDepletedSignature OnStaminaEmpty;
#pragma endregion

#pragma region Public Getters and Setters
	// Return current stamina
	UFUNCTION(BlueprintPure, Category = "Stamina")
	int32 GetStamina();

	// Return standard stamina recuperation factor
	UFUNCTION(BlueprintPure, Category = "Stamina")
	int32 GetStaminaRecuperationFactor();

	// Set standard stamina recuperation factor
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetStaminaRecuperationFactor(int32 NewFactor);

	// Return max stamina
	UFUNCTION(BlueprintPure, Category = "Stamina")
	int32 GetMaxStamina();

	// Return the value of bIsRunning
	UFUNCTION(BlueprintPure, Category = "Stamina")
	bool GetIsRunning();
	
	// Controls whether the player is running or walking
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetRunning(bool bNewValue);

	// Sets bHasRan, which controls running stamina drain
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetHasRan(bool bNewValue);

	// Modify dashing stamina cost
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetDashStaminaCost(int32 NewCost);

	// Return dashing stamina cost
	UFUNCTION(BlueprintPure, Category = "Stamina")
	int32 GetDashStaminaCost();

	// Return running stamina cost
	UFUNCTION(BlueprintPure, Category = "Stamina")
	int32 GetRunStaminaCost();

	// Modify running stamina cost
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetRunStaminaCost(int32 NewCost);
	
	// Modify boosted stamina recovery (while crouching)
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetStaminaRebate(int32 NewRebate);
#pragma endregion

private:
#pragma region Properties
	UPROPERTY(EditAnywhere)
	int32 MaxStamina = 100;

	int32 CurrentStamina;
	int32 StaminaRecuperationFactor = 1;

	UPROPERTY(EditAnywhere)
	int32 DashStaminaCost = 5;

	UPROPERTY(EditAnywhere)
	int32 RunStaminaCost = 3;
	
	UPROPERTY(EditAnywhere)
	int32 RestStaminaRebate = 4;
#pragma endregion
		
	UPROPERTY()
	APlayerCharacter* MyOwner = nullptr;

#pragma region Booleans
	bool bIsRunning = false;
	bool bHasRan = false;
	bool bHasDashed = false;
#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|Speed")
	float NormalMaxWalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|Speed")
	float RunningMaxWalkSpeed = 500.0f;
};
