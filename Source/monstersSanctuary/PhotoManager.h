// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI_CreatureCharacterInterface.h"
#include "AI_CreatureInteractableInterface.h"
#include "UnrealClient.h"
#include "PhotoManager.generated.h"

USTRUCT(BlueprintType)
struct FEvaluateSceneResult {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float value;
};

/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, BlueprintType)
class MONSTERSSANCTUARY_API UPhotoManager : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCreatureStats statsEvaluateMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCreatureStats statsEvaluateAdder;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float statsMedianMultiplier;
	

	UFUNCTION(BlueprintCallable)
	void EvaluateScene(
		////inputs
		TArray<AActor*> allCreatures,
		//TArray<AActor*> allInteractable,
		//TArray<AActor*> allFornitures,
		// outputs
		FEvaluateSceneResult& result
	);
};