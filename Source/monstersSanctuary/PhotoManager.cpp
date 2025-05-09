// Fill out your copyright notice in the Description page of Project Settings.

#include "PhotoManager.h"

void UPhotoManager::EvaluateScene
(
	////inputs
	TArray<AActor*> allCreatures,
	TArray<AActor*> allInteractables,
	TArray<AActor*> allFurnitures,
	// outputs
	FEvaluateSceneResult& result	
) 
{
	result = FEvaluateSceneResult(); // create new Scene result



	// init scene result
	result.value = 0; // can be only positive and defines the intensity of patreon donations
	result.brokenStructureVisible = false;
	result.lowLevelStructureVisible = false;
	result.noFurnitureVisible = false;
	result.sadCreatureVisible = false;
	result.CreatureIsntDoingAnything = false;
	result.needFoodCreatureVisible = false;
	result.needWaterCreatureVisible = false;
	result.needBedCreatureVisible = false;
	result.needCleanCreatureVisible = false;
	result.noCreatureInside = true;



	// evaluate creatures inside
	float statsMedian = 0;
	int creatureNumber = 0;
	for (AActor* creature : allCreatures) {
		if (creature == NULL || !creature->WasRecentlyRendered(0.5)) {
			continue; // skip the creature if it is not rendered
		}
		FCreatureStats stats;
		IAI_CreatureCharacterInterface* creatureInterface = Cast<IAI_CreatureCharacterInterface>(creature);
		creatureInterface->Execute_GetCreatureStats(creature, stats);

		float currentMedian = 0;
		float finalMultiplier = 1;

		// hint construction
		if (stats.hunger < statsMinimumToConsiderPositive.hunger) { result.needFoodCreatureVisible = true; finalMultiplier *= multiplierIfCreatureHasNegativeProperty; }
		if (stats.thirst < statsMinimumToConsiderPositive.thirst) { result.needWaterCreatureVisible = true; finalMultiplier *= multiplierIfCreatureHasNegativeProperty; }
		if (stats.slumber < statsMinimumToConsiderPositive.slumber) { result.needBedCreatureVisible = true; finalMultiplier *= multiplierIfCreatureHasNegativeProperty; }
		if (stats.cleanness < statsMinimumToConsiderPositive.cleanness) { result.needCleanCreatureVisible = true; finalMultiplier *= multiplierIfCreatureHasNegativeProperty; }
		if (stats.happyness < statsMinimumToConsiderPositive.happyness) { result.sadCreatureVisible = true; finalMultiplier *= multiplierIfCreatureHasNegativeProperty; }

		// median cunstruction
		currentMedian += ((stats.hunger + statsEvaluateAdder.hunger) * statsEvaluateMultiplier.hunger);
		currentMedian += ((stats.thirst + statsEvaluateAdder.thirst) * statsEvaluateMultiplier.thirst);
		currentMedian += ((stats.slumber + statsEvaluateAdder.slumber) * statsEvaluateMultiplier.slumber);
		currentMedian += ((stats.cleanness + statsEvaluateAdder.cleanness) * statsEvaluateMultiplier.cleanness);
		currentMedian += ((stats.happyness + statsEvaluateAdder.happyness) * statsEvaluateMultiplier.happyness);
		currentMedian /= 5.0;
		
		// well centered
		bool wellCentered = false;
		creatureInterface->Execute_IsInsideCenter(creature, wellCentered);
		if (!wellCentered) currentMedian *= outsideCenterMultiplierCreature;
		else result.noCreatureInside = false;
		currentMedian *= finalMultiplier;
		UE_LOG(LogTemp, Warning, TEXT("creature %s evaluated with : %f"), *creature->GetName(), currentMedian);
		
		statsMedian += currentMedian;
		creatureNumber++;
	}
	if (creatureNumber != 0) statsMedian /= creatureNumber;
	else result.noCreatureInside = true;
	statsMedian *= statsMedianMultiplier;
	UE_LOG(LogTemp, Warning, TEXT("stats median for creatures : %f"), statsMedian);
	// final value adding
	result.value += statsMedian;
	result.value += bonusForeachCreature * creatureNumber;



	// evaluate interactable inside
	float interactableMedian = 0;
	int interactableNumber = 0;
	for (AActor* interactable : allInteractables) {
		if (interactable == NULL || !interactable->WasRecentlyRendered(0.5)) {
			continue; // skip the interactable if it is not rendered
		}
		float health;
		bool broken;
		int level;
		IAI_CreatureInteractableInterface* interactableInterface = Cast<IAI_CreatureInteractableInterface>(interactable);
		interactableInterface->Execute_GetInteractableStats(interactable, health, broken, level);

		float currentValue = 0;

		// median construction
		currentValue = (health * interactableHealthValueMultiplier);
		currentValue += level * interactableLevelMultiplier;
		if (level == 1) {
			result.lowLevelStructureVisible = true;
		}
		if (broken) { 
			currentValue *= interactableIsBrokenMultiplier; 
			result.brokenStructureVisible = true;
		}
		
		// well centered
		bool wellCentered = false;
		interactableInterface->Execute_IsInsideCenter(interactable, wellCentered);
		if (!wellCentered) currentValue *= outsideCenterMultiplierCreature;
		UE_LOG(LogTemp, Warning, TEXT("interactable %s evaluated with : %f"), *interactable->GetName(), currentValue);
		
		interactableMedian += currentValue;
		interactableNumber++;
	}
	if (interactableNumber != 0) interactableMedian /= interactableNumber;
	UE_LOG(LogTemp, Warning, TEXT("interactable median median for interactables : %f"), interactableMedian);
	// final value adding
	result.value += interactableMedian;
	result.value += bonusForeachInteractable * interactableNumber;



	// evaluate forniture inside
	int furnitureNumber = 0;
	for (AActor* furniture : allFurnitures) {
		if (furniture == NULL || !furniture->WasRecentlyRendered(0.5)) {
			continue; // skip the forniture if it is not rendered
		}
		furnitureNumber++;
	}
	if (furnitureNumber == 0) result.noFurnitureVisible = true;
	// final value adding
	result.value += bonusForeachFurniture * creatureNumber;
	


	// last checks
	if (result.noCreatureInside) result.value = 0; // if no creature inside the value will be 0
	// limit the value in a positive number
	if (result.value < 0) result.value = 0;
	UE_LOG(LogTemp, Warning, TEXT("calculated final value : %f"), result.value);



	//// image generation
	//// Generate a filename based on the current date
	//const FDateTime Now = FDateTime::Now();
	//// store screenshot in Project directory next to main UProject/EXE based on the build type
	//#if WITH_EDITOR
	//const FString ImageDirectory = FString::Printf(TEXT("%s%s"), *FPaths::ProjectDir(), TEXT("Screenshots"));
	//#else
	//const FString ImageDirectory = FString::Printf(TEXT("%s/../%s"), *FPaths::ProjectDir(), TEXT("Screenshots"));
	//#endif
	//const FString ImageFilename = FString::Printf(TEXT("%s/Screenshot_%d%02d%02d_%02d%02d%02d_%03d.png"), *ImageDirectory, Now.GetYear(), Now.GetMonth(), Now.GetDay(), Now.GetHour(), Now.GetMinute(), Now.GetSecond(), Now.GetMillisecond());
	//FScreenshotRequest::RequestScreenshot(ImageFilename, false, false);
	//UE_LOG(LogTemp, Warning, TEXT("Saved screenshot at; %s"), *ImageFilename);

	//UGameViewportClient::OnScreenshotCaptured().AddUFunction();


	//FString FullPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*ImageFilename);
	//result.photoFileName = FullPath;

	//UGameViewportClient* Viewport = ()->GetGameViewport();
	//FIntPoint ViewSize = Viewport->Viewport->GetSizeXY();
	// 
	// returns a positive value old(between -1 and 1) to evaluate the scene ( checking also if the creatures and the interactables are inside the photo )
}
//
//UTexture2D* UPhotoManager::ConverResultImageToTexture2D
//(
//	//inputs
//	FString imagePath
//) 
//{
//	FVector2D ViewPortSize = FVector2D(1, 1);
//
//	if (GEngine && GEngine->GameViewport)
//	{
//		GEngine->GameViewport->GetViewportSize( /*out*/ViewPortSize);
//	}
//
//	UE_LOG(LogTemp, Warning, TEXT("viewport size : %f,%f"), ViewPortSize[0], ViewPortSize[1]);
//
//	TArray<uint8> LoadedData;
//	FFileHelper::LoadFileToArray(LoadedData, *imagePath);
//	UTexture2D* text = UTexture2D::CreateTransient(ViewPortSize[0], ViewPortSize[1]);
//
//	return text;
//}
//
//void UPhotoManager::ScreenShotCaptured(int32 Width, int32 Height, const TArray<FColor>& Bitmap) 
//{
//	//UTexture2D* text = UTexture2D::createtran
//}
//
//static void ScreenshotCaptured(int32 Width, int32 Height, const TArray<FColor>& Bitmap) 
//{
//
//}