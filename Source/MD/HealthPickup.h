// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class MD_API AHealthPickup : public APickup
{
	GENERATED_BODY()
	
	
public:
	// Sets default values for this actor's properties
	AHealthPickup();

	/** Override WasCollected function - use Implementation becuase blueprint native event*/
	void WasCollected_Implementation() override;

	/**Acessor for the energy power level**/
	float GetHealthValue();

protected:

	/** Amount of power energy pick up gives**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (BlueprintProtected = "true"))
		float HealthValue;


	
};
