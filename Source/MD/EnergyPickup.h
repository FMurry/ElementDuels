// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "EnergyPickup.generated.h"

/**
 * 
 */
UCLASS()
class MD_API AEnergyPickup : public APickup
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AEnergyPickup();

	/** Override WasCollected function - use Implementation becuase blueprint native event*/
	void WasCollected_Implementation() override;

	/**Acessor for the energy power level**/
	float GetEnergyPower();
	
protected:

	/** Amount of power energy pick up gives**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	float EnergyPower;

	
};
