// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shop.generated.h"



USTRUCT(BlueprintType)
struct FShopItem : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString Name;
	UPROPERTY(EditAnywhere)
	float Price;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlaceableItem;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlaceableSlot; // must be of type BP_PlaceableSlot
	UPROPERTY(EditAnywhere)
	FString UpgradeOfRowName; // if UpgradeOfRowName is not empty, this ShopItem is an upgrade of another ShopItem

	bool operator==(const FShopItem& obj) const;
};



/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, BlueprintType)
class MONSTERSSANCTUARY_API UShop : public UObject
{
	GENERATED_BODY()
protected:
	UFUNCTION()
	TArray<FShopItem> LoadPurchasedItems();
	UFUNCTION()
	TArray<FShopItem> GenerateItemsInShop();
public:
	UShop();
	~UShop();

	UPROPERTY(EditAnywhere)
	UDataTable* PurchaseableItems;
	UPROPERTY(EditAnywhere)
	TArray<FShopItem> ItemsInShop;
	UPROPERTY(EditAnywhere)
	TArray<FShopItem> PurchasedItems;

	UFUNCTION(BlueprintCallable)
	void Buy(FString RowName);
};


