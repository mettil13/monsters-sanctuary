// Fill out your copyright notice in the Description page of Project Settings.

#include "PhotoManager.h"

void UPhotoManager::EvaluateScene(
	////inputs
	TArray<AActor*> allCreatures,
	//TArray<AActor*> allInteractable,
	//TArray<AActor*> allFornitures,
	// outputs
	FEvaluateSceneResult& result
) 
{
	FCreatureStats stats = FCreatureStats();
	result = FEvaluateSceneResult();
	result.value = 0;

	float statsMedian = 0;

	for (AActor* creature : allCreatures) {
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *creature->GetName());
		IAI_CreatureCharacterInterface* creatureInterface = Cast<IAI_CreatureCharacterInterface>(creature);
		creatureInterface->Execute_GetCreatureStats(creature, stats);

		float currentMedian = 0;

		currentMedian += (stats.hunger * statsEvaluateMultiplier.hunger) + statsEvaluateAdder.hunger;
		currentMedian += (stats.thirst * statsEvaluateMultiplier.thirst) + statsEvaluateAdder.thirst;
		currentMedian += (stats.slumber * statsEvaluateMultiplier.slumber) + statsEvaluateAdder.slumber;
		currentMedian += (stats.cleanness * statsEvaluateMultiplier.cleanness) + statsEvaluateAdder.cleanness;
		currentMedian += (stats.happyness * statsEvaluateMultiplier.happyness) + statsEvaluateAdder.happyness;
		currentMedian /= 5.0;
		UE_LOG(LogTemp, Warning, TEXT("creature %s evaluated with : %f"), *creature->GetName(), currentMedian);
		statsMedian += currentMedian;
	}

	statsMedian /= allCreatures.Num();
	statsMedian *= statsMedianMultiplier;






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

