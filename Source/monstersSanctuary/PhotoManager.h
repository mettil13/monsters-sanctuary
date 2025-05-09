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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EvaluateHints")
	bool brokenStructureVisible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EvaluateHints")
	bool lowLevelStructureVisible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EvaluateHints")
	bool noFurnitureVisible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EvaluateHints")
	bool CreatureIsntDoingAnything;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EvaluateHints")
	bool sadCreatureVisible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EvaluateHints")
	bool needBedCreatureVisible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EvaluateHints")
	bool needWaterCreatureVisible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EvaluateHints")
	bool needFoodCreatureVisible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EvaluateHints")
	bool needCleanCreatureVisible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EvaluateHints")
	bool noCreatureInside;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Photo settings")
	FString photoFileName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Photo settings")
	UMaterialInstanceDynamic* photoMaterial;
};

/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, BlueprintType)
class MONSTERSSANCTUARY_API UPhotoManager : public UObject
{
	GENERATED_BODY()
	
public:
	
	//creature evaluation

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Creature evaluation")
	FCreatureStats statsEvaluateMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature evaluation")
	FCreatureStats statsEvaluateAdder;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature evaluation")
	float statsMedianMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature evaluation")
	FCreatureStats statsMinimumToConsiderPositive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature evaluation")
	float bonusForeachCreature;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature evaluation")
	float outsideCenterMultiplierCreature;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature evaluation")
	float multiplierIfCreatureHasNegativeProperty;

	// interactable evaluation

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable evaluation")
	float interactableHealthValueMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable evaluation")
	float interactableIsBrokenMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable evaluation")
	float interactableLevelMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable evaluation")
	float bonusForeachInteractable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable evaluation")
	float outsideCenterMultiplierInteractable;

	// furniture evaluation

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Furniture evaluation")
	float bonusForeachFurniture;
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Furniture evaluation")
	float outsideCenterMultiplierFurniture;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "All results")
	TArray<FEvaluateSceneResult> results;

	UFUNCTION(BlueprintCallable)
	void EvaluateScene(
		//inputs
		TArray<AActor*> allCreatures,
		TArray<AActor*> allInteractables,
		TArray<AActor*> allFurnitures,
		//outputs
		FEvaluateSceneResult& result
	);

	//UFUNCTION(BlueprintCallable)
	//UTexture2D* ConverResultImageToTexture2D(
	//	//inputs
	//	FString imagePath
	//);

	//UFUNCTION(BlueprintCallable)
	//void ScreenShotCaptured(int32 Width, int32 Height, const TArray<FColor>& Bitmap);
};

//static void ScreenshotCaptured(int32 Width, int32 Height, const TArray<FColor>& Bitmap);