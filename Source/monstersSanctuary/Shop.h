// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shop.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, BlueprintType)
class MONSTERSSANCTUARY_API UShop : public UObject
{
	GENERATED_BODY()
public:
	UShop();
	~UShop();

	UPROPERTY(EditAnywhere)
	UDataTable* PurchaseableItems;
	UPROPERTY(EditAnywhere)
	UDataTable* PurchasedItems;

	UFUNCTION(BlueprintCallable)
	void Buy(FString RowName);
};


USTRUCT(BlueprintType)
struct FShopItem : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString Name;
	UPROPERTY(EditAnywhere)
	float Price;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> InstantiableItem;
	//UPROPERTY(EditAnywhere)
	//TSubclassOf<COSA> InstantiableItem;
};
