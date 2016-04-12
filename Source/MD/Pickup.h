// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class MD_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Retunr mesh for pickup*/
	FORCEINLINE class UStaticMeshComponent* GetMesh() const {
		return PickupMesh;
	}

	/** Return if pickup is active*/
	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();
		
	/**Allows other classes to change whether pickup is active or not */
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool state);

	/** Function called when pickup is collected*/
	UFUNCTION(BlueprintNativeEvent)
	void WasCollected();
	virtual void WasCollected_Implementation();

protected:
	/** True when pickup can be used and false when deactivated*/
	bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RotationRate;
	
private:

	/** Static Mesh to represent the pickup item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

	
	
};
