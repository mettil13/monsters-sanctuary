// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"

UShop::UShop()
{


}

UShop::~UShop()
{
}

void UShop::Buy(FString RowName)
{
	FShopItem* rowToPurchase = PurchaseableItems->FindRow<FShopItem>((FName)RowName, "");

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

	
	return itemsInShop;
}

bool FShopItem::operator==(const FShopItem& obj) const
{
	return (Name == obj.Name &&
		Price == obj.Price &&
		PlaceableItem == obj.PlaceableItem &&
		PlaceableSlot == obj.PlaceableSlot &&
		UpgradeOfRowName == obj.UpgradeOfRowName);
}

