// Fill out your copyright notice in the Description page of Project Settings.

#include "PhotoManager.h"

void UPhotoManager::EvaluateScene(
	////inputs
	TArray<AActor*> allCreatures,
	TArray<AActor*> allInteractable,
	//TArray<AActor*> allFornitures,
	// outputs
	FEvaluateSceneResult& result
) 
{
	
	result = FEvaluateSceneResult();
	result.value = 0; // can be only positive and defines the intensity of patreon donations
	result.brokenStructureVisible = false;
	result.lowLevelStructureVisible = false;
	result.sadCreatureVisible = false;
	result.CreatureIsntDoingAnything = false;
	result.needFoodCreatureVisible = false;
	result.needWaterCreatureVisible = false;
	result.needBedCreatureVisible = false;
	result.needCleanCreatureVisible = false;

	// evaluate creatures inside
	float statsMedian = 0;
	int c = 0;
	for (AActor* creature : allCreatures) {
		if (!creature->WasRecentlyRendered(0.5)) {
			continue; // skip the creature if it is not rendered
		}
		FCreatureStats stats;
		IAI_CreatureCharacterInterface* creatureInterface = Cast<IAI_CreatureCharacterInterface>(creature);
		creatureInterface->Execute_GetCreatureStats(creature, stats);

		float currentMedian = 0;

		// hint construction
		if (stats.hunger < statsMinimumToConsiderPositive.hunger) result.needFoodCreatureVisible = true;
		if (stats.thirst < statsMinimumToConsiderPositive.thirst) result.needWaterCreatureVisible = true;
		if (stats.slumber < statsMinimumToConsiderPositive.slumber) result.needBedCreatureVisible = true;
		if (stats.cleanness < statsMinimumToConsiderPositive.cleanness) result.needCleanCreatureVisible = true;
		if (stats.happyness < statsMinimumToConsiderPositive.happyness) result.sadCreatureVisible = true;

		// median cunstruction
		currentMedian += (stats.hunger * statsEvaluateMultiplier.hunger) + statsEvaluateAdder.hunger;
		currentMedian += (stats.thirst * statsEvaluateMultiplier.thirst) + statsEvaluateAdder.thirst;
		currentMedian += (stats.slumber * statsEvaluateMultiplier.slumber) + statsEvaluateAdder.slumber;
		currentMedian += (stats.cleanness * statsEvaluateMultiplier.cleanness) + statsEvaluateAdder.cleanness;
		currentMedian += (stats.happyness * statsEvaluateMultiplier.happyness) + statsEvaluateAdder.happyness;
		currentMedian /= 5.0;
		UE_LOG(LogTemp, Warning, TEXT("creature %s evaluated with : %f"), *creature->GetName(), currentMedian);
		statsMedian += currentMedian;
		c++;
	}
	if (c != 0) statsMedian /= c;
	statsMedian *= statsMedianMultiplier;
	UE_LOG(LogTemp, Warning, TEXT("stats median for creatures : %f"), statsMedian);

	// evaluate interactable inside
	float interactableMedian = 0;
	c = 0;
	for (AActor* interactable : allInteractable) {
		if (interactable == NULL || !interactable->WasRecentlyRendered(0.5)) {
			continue; // skip the creature if it is not rendered
		}
		float health;
		bool broken;
		int level;
		IAI_CreatureInteractableInterface* interactableInterface = Cast<IAI_CreatureInteractableInterface>(interactable);
		interactableInterface->Execute_GetInteractableStats(interactable, health, broken, level);

		float currentValue = 0;

		
		currentValue = (health * interactableHealthValueMultiplier);
		if (broken) { 
			currentValue *= interactableIsBrokenMultiplier; 
			result.brokenStructureVisible = true;
		}
		currentValue += level * interactableLevelMultiplier;
		if (level == 1) {
			result.lowLevelStructureVisible = true;
		}
		UE_LOG(LogTemp, Warning, TEXT("interactable %s evaluated with : %f"), *interactable->GetName(), currentValue);
		interactableMedian += currentValue;
		c++;
	}


	result.value = statsMedian + interactableMedian;

	// Generate a filename based on the current date
	const FDateTime Now = FDateTime::Now();
	// store screenshot in Project directory next to main UProject/EXE based on the build type
	#if WITH_EDITOR
	const FString ImageDirectory = FString::Printf(TEXT("%s%s"), *FPaths::ProjectDir(), TEXT("Screenshots"));
	#else
	const FString ImageDirectory = FString::Printf(TEXT("%s/../%s"), *FPaths::ProjectDir(), TEXT("Screenshots"));
	#endif
	const FString ImageFilename = FString::Printf(TEXT("%s/Screenshot_%d%02d%02d_%02d%02d%02d_%03d.png"), *ImageDirectory, Now.GetYear(), Now.GetMonth(), Now.GetDay(), Now.GetHour(), Now.GetMinute(), Now.GetSecond(), Now.GetMillisecond());

	FScreenshotRequest::RequestScreenshot(ImageFilename, false, false);
	UE_LOG(LogTemp, Warning, TEXT("Saved screenshot at; %s"), *ImageFilename);
	// returns a value between -1 and 1 to evaluate the scene ( checking also if the creatures and the interactables are inside the photo )
	//FScreenshotRequest.RequestScreenshot(false);
	//FScreenshotRequest::RequestScreenshot(false);
}

