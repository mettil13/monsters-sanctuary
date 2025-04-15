// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"

UShop::UShop()
{
	
}

UShop::~UShop()
{
}

void UShop::Init()
{
	RefreshItemsInShop();
	RefreshPlaceableSlotsInScene();
}

bool UShop::Buy(FShopItem item)
{

	if (ItemsInShop.IsEmpty() || !ItemsInShop.Contains(item)) {
		return false;
	}

	ItemsInShop.Remove(item);
	PurchasedItems.Add(item);

	AActor* itemPlaceableSlot = SearchInPlaceableSlotListByName(item.PlaceableSlotName);
	if (itemPlaceableSlot == NULL) {
		RefreshPlaceableSlotsInScene();
		itemPlaceableSlot = SearchInPlaceableSlotListByName(item.PlaceableSlotName);
	}

	if (itemPlaceableSlot && item.PlaceableItem) {
		if (itemPlaceableSlot->GetClass()->ImplementsInterface(UPlaceableSlotInterface::StaticClass())) {
			IPlaceableSlotInterface* itemPlaceableSlotInterface = Cast<IPlaceableSlotInterface>(itemPlaceableSlot);
			itemPlaceableSlotInterface->Execute_SpawnPlaceable(itemPlaceableSlot, item.PlaceableItem);
		}
	}
	RefreshItemsInShop();

	return true;

}

TArray<FShopItem> UShop::LoadPurchasedItems()
{
	TArray<FShopItem> itemsInShop;
	return itemsInShop;
}

void UShop::RefreshItemsInShop() {
	ItemsInShop = GenerateItemsInShop();
}

void UShop::RefreshPlaceableSlotsInScene()
{
	PlaceableSlotsInScene = GeneratePlaceableSlotsList();
}

TArray<FShopItem> UShop::GenerateItemsInShop()
{
	TArray<FShopItem> itemsInShop; 
	
	TArray<FShopItem*> items; 
	PurchaseableItems->GetAllRows("", items);

	for (FShopItem const* i : items) {

		if (PurchasedItems.Contains(*i) == false) {
			itemsInShop.Add(*i);
		}
		
	}

	for (int i = itemsInShop.Num() - 1; i >= 0; i--) {
		FString upgradeName = itemsInShop[i].UpgradeOfRowName;
		if (!IsValidUpgrade(upgradeName, itemsInShop)) {
			itemsInShop.RemoveAt(i);
		}
	}

	
	return itemsInShop;
}

TArray<AActor*> UShop::GeneratePlaceableSlotsList()
{
	TArray<AActor*> placeableSlotActors;
	UGameplayStatics::GetAllActorsWithInterface(this, UPlaceableSlotInterface::StaticClass(), placeableSlotActors);

	return placeableSlotActors;
}

AActor* UShop::SearchInPlaceableSlotListByName(FString name)
{
	for (AActor* slot : PlaceableSlotsInScene) {
		//UE_LOG(LogTemp, Warning, TEXT("slot: %s"), *(slot->GetActorNameOrLabel()));
		if (slot->GetActorNameOrLabel() == name) {
			return slot;
		}
	}

	return NULL;
}

bool UShop::IsValidUpgrade(FString upgradeName, TArray<FShopItem> itemsInShop) {
	if (upgradeName == "") {
		return true;
	}

	for (FShopItem i : itemsInShop) {
		if (i.Name == upgradeName) {
			return true;
		}
	}
	return false;
}

bool FShopItem::operator==(const FShopItem& obj) const
{
	return (Name == obj.Name &&
		Price == obj.Price &&
		PlaceableItem == obj.PlaceableItem &&
		PlaceableSlotName == obj.PlaceableSlotName &&
		UpgradeOfRowName == obj.UpgradeOfRowName);
}

