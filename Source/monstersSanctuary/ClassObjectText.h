// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ClassObjectText.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, BlueprintType)
class MONSTERSSANCTUARY_API UClassObjectText : public UObject 
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float FloatBello;
};

USTRUCT(BlueprintType)
struct FPeppino {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float FloatBruttino;
};