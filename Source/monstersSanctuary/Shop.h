// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableSlotInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Shop.generated.h"



USTRUCT(BlueprintType)
struct FShopItem : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Price;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActor> PlaceableItem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString PlaceableSlotName;
	//TScriptInterface<IPlaceableSlotInterface> PlaceableSlot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
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
	UPROPERTY()
	TArray<AActor*> PlaceableSlotsInScene;
	
	UFUNCTION()
	TArray<FShopItem> LoadPurchasedItems();
	UFUNCTION()
	TArray<FShopItem> GenerateItemsInShop();
	UFUNCTION()
	TArray<AActor*> GeneratePlaceableSlotsList();
	UFUNCTION()
	AActor* SearchInPlaceableSlotListByName(FString name);
	UFUNCTION()
	bool IsValidUpgrade(FString upgradeName, TArray<FShopItem> itemsInShop);
public:
	UShop();
	~UShop();

	UPROPERTY(EditAnywhere)
	UDataTable* PurchaseableItems;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FShopItem> ItemsInShop;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FShopItem> PurchasedItems;

	UFUNCTION(BlueprintCallable)
	void Init();
	UFUNCTION(BlueprintCallable)
	bool Buy(FShopItem item);
	UFUNCTION(BlueprintCallable)
	void RefreshItemsInShop();
	UFUNCTION(BlueprintCallable)
	void RefreshPlaceableSlotsInScene();
};


