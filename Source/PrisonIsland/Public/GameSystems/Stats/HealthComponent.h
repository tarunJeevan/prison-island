#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UDamageType;
class AController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOwnerDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, 
											UHealthComponent*, HealthComponent,
											float, Health,
											float, DamageAmount,
											const UDamageType*, DamageType,
											AController*, InstigatedBy,
											AActor*, DamageCauser);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRISONISLAND_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;
#pragma endregion

	UFUNCTION(BlueprintCallable)
	void HandleDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health|Max Health")
	void SetMaxHealth(float NewMaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Health|Current Health")
	void RestoreToFullHealth();

public:
#pragma region Delegates
	UPROPERTY(BlueprintAssignable)
	FOnOwnerDeath OnPlayerDeath;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;
#pragma endregion

#pragma region Getters
	UFUNCTION(BlueprintPure, Category = "Health|Max Health")
	float GetMaxHealth();

	UFUNCTION(BlueprintPure, Category = "Health|Current Health")
	float GetHealth();
#pragma endregion
};
