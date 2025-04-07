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
}

void UShop::Buy(FString RowName)
{
	FShopItem* rowToPurchase = PurchaseableItems->FindRow<FShopItem>((FName)RowName, "");
	if (rowToPurchase == NULL || ItemsInShop.IsEmpty()) {
		return;
	}

	PurchasedItems.Add(*rowToPurchase);
	RefreshItemsInShop();


	/*
	FShopItem* rowToPurchase = PurchaseableItems->FindRow<FShopItem>((FName)RowName, "");
	PurchaseableItems->RemoveRow((FName)RowName);
	// Remove money
	UE_LOG(LogTemp, Warning, TEXT("removed %s"), *RowName);
	PurchasedItems->AddRow((FName)RowName, *rowToPurchase);

	TArray<FShopItem*> items;
	PurchaseableItems->GetAllRows("", items);
	for (FShopItem* item : items) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(item->Name));

	}
	*/
}

TArray<FShopItem> UShop::LoadPurchasedItems()
{
	TArray<FShopItem> itemsInShop;
	return itemsInShop;
}

void UShop::RefreshItemsInShop() {
	ItemsInShop = GenerateItemsInShop();
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
		PlaceableSlot == obj.PlaceableSlot &&
		UpgradeOfRowName == obj.UpgradeOfRowName);
}

