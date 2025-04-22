// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI_CreatureCharacter.h"
#include "AI_CreatureCharacterInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UAI_CreatureCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class IAI_CreatureCharacterInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void GetCreatureStats(FCreatureStats& result);
};
