// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Printone.generated.h"

UCLASS()
class MONSTERSSANCTUARY_API APrintone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrintone();

	UPROPERTY(EditAnywhere)
	float FloatToPrint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void PrintaCOSE();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
