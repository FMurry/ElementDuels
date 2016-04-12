// Fill out your copyright notice in the Description page of Project Settings.

#include "MD.h"
#include "Pickup.h"


// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	//Initial value is true at start
	bIsActive = true;
	
	//Create the static mesh component
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupMesh;

	RotationRate = 60.0f;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//Makes the pickup rotate on Z axis
	FRotator ActorRotation = GetActorRotation();
	ActorRotation.Yaw += RotationRate*DeltaTime;
	SetActorRotation(ActorRotation);

}

//Is pickup active?
bool APickup::IsActive() {
	return bIsActive;
}

//Set pickup state
void APickup::SetActive(bool state) {
	bIsActive = state;

}

void APickup::WasCollected_Implementation() {
	
	FString message = GetName();
	//UE_LOG(LogClass, Log, TEXT("You have collected %s"), *message);
}