// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "MDCharacter.generated.h"

UCLASS(config=Game)
class AMDCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/**Collection sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickups", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;

public:
	AMDCharacter();

	/**Called at beginning of play**/
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;


	/**Accessor for Initial Power**/
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetInitialPower();
	
	/**Accessor for current power**/
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetCurrentPower();

	/** Accessor for max power**/
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetMaxPower();
	
	/** Accessor for max health**/
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth();
	
	/**Function to update characters power
	*@param Amount to change power by (positive or negative)
	*/
	UFUNCTION(BlueprintCallable, Category = "Power")
	void UpdatePower(float change);

	/**Accessor for Initial Health**/
	UFUNCTION(BlueprintPure, Category = "Health")
		float GetInitalHealth();

	/**Accessor for current health**/
	UFUNCTION(BlueprintPure, Category = "Health")
		float GetCurrentHealth();

	/**Function to update characters health
	*@param Amount to change power by (positive or negative)
	*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		void UpdateHealth(float change);


protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/**Called when we press a key to collect any pickups inside collection sphere**/
	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void CollectPickups();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void Sprinting();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void NotSprinting();

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SwimUp();

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SwimDown();

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void Attack();
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	/**Power that player starts off with**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float InitialPower;

	/**Health that player starts off with**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Meta = (BlueprintProtected = "true"))
	float InitialHealth;

	/** Max Power of player**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float MaxPower;

	/** Max Health of player**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Meta = (BlueprintProtected = "true"))
	float MaxHealth;
	/** Base Speed of character**/
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Speed", Meta = (BlueprintProtected = "true"))
	float BaseSpeed;

	/** Increases speed (multiplier) of character (When power up is used) Example Wind and electricity might have speed factors**/
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Speed", Meta = (BlueprintProtected = "true"))
	float SpeedFactor;

	/**Visual Effect of becoming more powerful**/
	UFUNCTION(BlueprintImplementableEvent, Category = "Power")
	void PowerChangeEffect();

private:

	/**Current power level of our character**/
	UPROPERTY(VisibleAnywhere, Category = "Power")
	float CurrentPower;

	UPROPERTY(VisibleAnyWhere, Category = "Health")
	float CurrentHealth;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns CollectionSphere subObject **/
	FORCEINLINE class USphereComponent* GetCollectionSphereComponent() const { return CollectionSphere; }
};

