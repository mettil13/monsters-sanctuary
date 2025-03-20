// Fill out your copyright notice in the Description page of Project Settings.


#include "Printone.h"

// Sets default values
APrintone::APrintone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APrintone::BeginPlay()
{
	Super::BeginPlay();
	GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("Inizializzato"));
	GetActorLocation();
	PrintaCOSE();
}

// Called every frame
void APrintone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APrintone::PrintaCOSE() 
{
	UE_LOG(LogTemp, Warning, TEXT("L'ho fatta tutta mamma"));
}

