// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableSlotInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Shop.generated.h"



UENUM(BlueprintType)
enum ShopCategory
{
	FOOD UMETA(DisplayName = "FOOD"),
	WATER UMETA(DisplayName = "WATER"),
	BED UMETA(DisplayName = "BED"),
	WC UMETA(DisplayName = "WC"),
	TOY UMETA(DisplayName = "TOY"),
	DECORATION UMETA(DisplayName = "DECORATION")
};

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
	FDataTableRowHandle UpgradeOfRow; // if UpgradeOfRow is not null, this ShopItem is an upgrade of another ShopItem
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Level; // if UpgradeOfRow is not null, Level shows the level of the upgrade
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ShopCategory> Category;

	bool operator==(const FShopItem& obj) const;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuyItemDelegate, FDataTableRowHandle, item);

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
	TArray<FDataTableRowHandle> LoadPurchasedItems();
	UFUNCTION()
	TArray<FDataTableRowHandle> GenerateItemsInShop();
	UFUNCTION()
	TArray<AActor*> GeneratePlaceableSlotsList();
	UFUNCTION()
	AActor* SearchInPlaceableSlotListByName(FString name);
	UFUNCTION()
	bool IsValidUpgradeByName(FString upgradeName, TArray<FDataTableRowHandle> itemsInShop);
	UFUNCTION()
	bool IsValidUpgrade(FDataTableRowHandle upgrade, TArray<FDataTableRowHandle> itemsInShop);
public:
	UShop();
	~UShop();

	UPROPERTY(EditAnywhere)
	UDataTable* PurchaseableItems;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDataTableRowHandle> ItemsInShop;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDataTableRowHandle> PurchasedItems;
	UPROPERTY(EditAnywhere, BlueprintAssignable)
	FOnBuyItemDelegate OnBuyItem;

	UFUNCTION(BlueprintCallable)
	void Init();
	UFUNCTION(BlueprintCallable)
	bool Buy(FDataTableRowHandle item);
	UFUNCTION(BlueprintCallable)
	void RefreshItemsInShop();
	UFUNCTION(BlueprintCallable)
	void RefreshPlaceableSlotsInScene();
	UFUNCTION(BlueprintCallable)
	TArray<FDataTableRowHandle> GetPurchasedItemsWithoutUpgrades();
	UFUNCTION(BlueprintCallable)
	int GetNumberOfPurchasedItemsOfCategory(ShopCategory category);
};


