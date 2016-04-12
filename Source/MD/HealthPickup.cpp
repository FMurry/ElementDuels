// Fill out your copyright notice in the Description page of Project Settings.

#include "MD.h"
#include "HealthPickup.h"



//Constructor
AHealthPickup::AHealthPickup() {

	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetEnableGravity(false);

	HealthValue = 150.0f;

}

void AHealthPickup::WasCollected_Implementation() {

	// Use the base pickup behavior
	Super::WasCollected_Implementation();

	//Destroy the Healthball
	if (!AHealthPickup::IsPendingKill()) {
		//Destroy();
	}

}

/**Accessor for Health**/
float AHealthPickup::GetHealthValue() {
	return HealthValue;
}
