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

bool UShop::Buy(FDataTableRowHandle item)
{

	if (ItemsInShop.IsEmpty() || !ItemsInShop.Contains(item)) {
		return false;
	}

	ItemsInShop.Remove(item);
	PurchasedItems.Add(item);

	AActor* itemPlaceableSlot = SearchInPlaceableSlotListByName(item.GetRow<FShopItem>("")->PlaceableSlotName);
	if (itemPlaceableSlot == NULL) {
		RefreshPlaceableSlotsInScene();
		itemPlaceableSlot = SearchInPlaceableSlotListByName(item.GetRow<FShopItem>("")->PlaceableSlotName);
	}

	if (itemPlaceableSlot && item.GetRow<FShopItem>("")->PlaceableItem) {
		if (itemPlaceableSlot->GetClass()->ImplementsInterface(UPlaceableSlotInterface::StaticClass())) {
			IPlaceableSlotInterface* itemPlaceableSlotInterface = Cast<IPlaceableSlotInterface>(itemPlaceableSlot);
			itemPlaceableSlotInterface->Execute_SpawnPlaceable(itemPlaceableSlot, item.GetRow<FShopItem>("")->PlaceableItem);
		}
	}

	RefreshItemsInShop();
	OnBuyItem.Broadcast(item);
	return true;

}

TArray<FDataTableRowHandle> UShop::LoadPurchasedItems()
{
	TArray<FDataTableRowHandle> itemsInShop;
	return itemsInShop;
}

void UShop::RefreshItemsInShop() {
	ItemsInShop = GenerateItemsInShop();
}

void UShop::RefreshPlaceableSlotsInScene()
{
	PlaceableSlotsInScene = GeneratePlaceableSlotsList();
}

TArray<FDataTableRowHandle> UShop::GetPurchasedItemsWithoutUpgrades()
{
	TArray<FDataTableRowHandle> purchasedItemsWithoutUpgrades;
	for (FDataTableRowHandle const i : PurchasedItems) {
		if (i.GetRow<FShopItem>("")->UpgradeOfRow.IsNull()) {
			purchasedItemsWithoutUpgrades.Add(i);
		}
	}

	return purchasedItemsWithoutUpgrades;
}

TArray<FDataTableRowHandle> UShop::GenerateItemsInShop()
{
	TArray<FDataTableRowHandle> itemsInShop;
	
	TArray<FDataTableRowHandle*> items;
	TArray<FName> rowNames = PurchaseableItems->GetRowNames();
	for (FName name : rowNames) {
		FDataTableRowHandle* row = new FDataTableRowHandle();
		row->DataTable = PurchaseableItems;
		row->RowName = name;
		items.Add(row);
	}

	for (FDataTableRowHandle const* i : items) {

		if (PurchasedItems.Contains(*i) == false) {
			itemsInShop.Add(*i);
		}
		
	}

	for (int i = itemsInShop.Num() - 1; i >= 0; i--) {
		FDataTableRowHandle upgrade = itemsInShop[i].GetRow<FShopItem>("")->UpgradeOfRow;
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

bool UShop::IsValidUpgradeByName(FString upgradeName, TArray<FDataTableRowHandle> itemsInShop) {
	if (upgradeName == "") {
		return true;
	}

	for (FDataTableRowHandle i : itemsInShop) {
		if (i.GetRow<FShopItem>("")->Name == upgradeName) {
			return true;
		}
	}
	return false;
}

bool UShop::IsValidUpgrade(FDataTableRowHandle upgrade, TArray<FDataTableRowHandle> itemsInShop) {
	if (upgrade.IsNull()) {
		return true;
	}

	for (FDataTableRowHandle i : itemsInShop) {
		if (i == upgrade) {
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

