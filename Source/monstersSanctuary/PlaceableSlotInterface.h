// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableSlotInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPlaceableSlotInterface : public UInterface
{
	GENERATED_BODY()
};

class IPlaceableSlotInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SpawnPlaceable(TSubclassOf<class AActor> placeable);
};
