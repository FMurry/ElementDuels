// Fill out your copyright notice in the Description page of Project Settings.

#include "MD.h"
#include "SpawnVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pickup.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create box component to represent spawn volume
	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;

	//Handle Spawn Delay range
	SpawnDelayRangeLow = 30.0f;
	SpawnDelayRangeHigh = 60.0f;

	MaxPickups = 10;

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	//Sets Spawn Delay
	SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
}

// Called every frame
void ASpawnVolume::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


FVector ASpawnVolume::GetRandomPointInVolume() {
	FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
	FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;

	//Included KismetLibrary which allows us to get random point based off of origin and extent
	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);

}

void ASpawnVolume::SpawnPickup() {

	if (WhatToSpawn != NULL)
	{
		//Checks if world is valid
		UWorld* const World = GetWorld();
		if (World) {
			TArray<AActor*> CollectedActors;
			WhereToSpawn->GetOverlappingActors(CollectedActors);
			int actorcount = CollectedActors.Num();

			if (actorcount < MaxPickups) {

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;

				//Get a Random point to spawn
				FVector SpawnLocation = GetRandomPointInVolume();

				//Gets a random rotation
				FRotator SpawnRotation;
				SpawnRotation.Yaw = FMath::FRand()*360.0f;
				SpawnRotation.Pitch = FMath::FRand()*360.0f;
				SpawnRotation.Roll = FMath::FRand()*360.0f;

				//If less than 5 actors spawn pickup 
				//TODO: Array also pickups player actor create an array that only stores actors//
				if (CollectedActors.Num() < MaxPickups) {
					APickup* const SpawnedPickup = World->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
					UE_LOG(LogClass, Log, TEXT("There are %d actor(s)"), actorcount);
				}
				else {
					UE_LOG(LogClass, Log, TEXT("Too many pickups on field, there are %d actors the max is %d"), actorcount, MaxPickups);
				}

				//Sets Spawn Delay after one is spawned
				SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
				GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);

			}
		}
	}
}
