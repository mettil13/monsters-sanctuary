// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_CreatureInteractableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UAI_CreatureInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class IAI_CreatureInteractableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void GetInteractableStats(float& health, bool& isBroken, int& level);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void IsInsideCenter(bool& result);
};
