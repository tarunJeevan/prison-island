#include "Items/ItemActorBase.h"
#include "Player/PlayerCharacter.h"
#include "Items/ItemObjectBase.h"
#include "GameSystems/Inventory/InventoryComponent.h"

// Sets default values
AItemActorBase::AItemActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AItemActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableData = InstanceInteractableData;

	Mesh->SetCollisionProfileName("Interactable");

	if (ItemObject)
	{
		InteractableData = ItemObject->GetInteractionData();
		InstanceItemData = ItemObject->GetItemData();
	}
	else
		ItemObject = GetDefaultItemObject();
}

// Called every frame
void AItemActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UItemObjectBase* AItemActorBase::GetDefaultItemObject_Implementation()
{
	TObjectPtr<UItemObjectBase> NewItemObject = NewObject<UItemObjectBase>();
	return NewItemObject;
}

void AItemActorBase::BeginFocus()
{
	if (Mesh)
		Mesh->SetRenderCustomDepth(true);
}

void AItemActorBase::EndFocus()
{
	if (Mesh)
		Mesh->SetRenderCustomDepth(false);
}

void AItemActorBase::BeginInteract_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract on the interaction actor"));
}

void AItemActorBase::EndInteract_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract on the interaction actor"));
}

void AItemActorBase::Interact_Implementation(APlayerCharacter* Character)
{
	if (Character)
		if (Character->GetInventoryComponent()->TryAddItem(ItemObject))
			Destroy();
}

