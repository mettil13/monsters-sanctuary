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

TArray<FShopItem> UShop::GetPurchasedItemsWithoutUpgrades()
{
	TArray<FShopItem> purchasedItemsWithoutUpgrades;
	for (FShopItem const i : PurchasedItems) {
		if (i.UpgradeOfRow.IsNull()) {
			purchasedItemsWithoutUpgrades.Add(i);
		}
	}

	return purchasedItemsWithoutUpgrades;
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
		/*
		FString upgradeName = itemsInShop[i].UpgradeOfRowName;
		if (!IsValidUpgrade(upgradeName, PurchasedItems)) {
			itemsInShop.RemoveAt(i);
		}
		*/

		FDataTableRowHandle upgrade = itemsInShop[i].UpgradeOfRow;
		if (!IsValidUpgrade(upgrade, PurchasedItems)) {
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

bool UShop::IsValidUpgradeByName(FString upgradeName, TArray<FShopItem> itemsInShop) {
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

bool UShop::IsValidUpgrade(FDataTableRowHandle upgrade, TArray<FShopItem> itemsInShop) {
	if (upgrade.IsNull()) {
		return true;
	}

	for (FShopItem i : itemsInShop) {
		if (i == *upgrade.GetRow<FShopItem>("")) {
			return true;
		}
	}
	return false;
}

bool FShopItem::operator==(const FShopItem& obj) const
{
	return (Name == obj.Name &&
		Price == obj.Price &&
		Description == obj.Description &&
		Thumbnail == obj.Thumbnail &&
		PlaceableItem == obj.PlaceableItem &&
		PlaceableSlotName == obj.PlaceableSlotName &&
		UpgradeOfRow == obj.UpgradeOfRow &&
		Category == obj.Category);
}

