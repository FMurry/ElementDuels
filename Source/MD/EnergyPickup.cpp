// Fill out your copyright notice in the Description page of Project Settings.

#include "MD.h"
#include "EnergyPickup.h"




//Constructor
AEnergyPickup::AEnergyPickup() {

	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetEnableGravity(false);

	EnergyPower = 150.0f;

}

void AEnergyPickup::WasCollected_Implementation() {

	// Use the base pickup behavior
	Super::WasCollected_Implementation();

	//Destroy the Energyball
		//Destroy();
	
}

/**Accessor for power**/
float AEnergyPickup::GetEnergyPower() {
	return EnergyPower;
}