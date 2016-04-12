// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MD.h"
#include "MDCharacter.h"
#include "Pickup.h"
#include "EnergyPickup.h"
#include "HealthPickup.h"

//////////////////////////////////////////////////////////////////////////
// AMDCharacter

AMDCharacter::AMDCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create the Collection Sphere

	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(200.0f);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Initial Power level of character

	InitialPower = 0.0f;
	CurrentPower = InitialPower;

	InitialHealth = 2000.0f;
	CurrentHealth = InitialHealth;
	BaseSpeed = 150.0f;
	SpeedFactor = 8.0f;

	MaxHealth = 2000.0f;
	MaxPower = 1000.0f;

	//Initial Set Character movement speed
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
}

// Called when the game starts or when spawned
void AMDCharacter::BeginPlay() {
	Super::BeginPlay();
	PowerChangeEffect();
}

// Called every frame
void AMDCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CollectPickups();
}
//////////////////////////////////////////////////////////////////////////
// Input

void AMDCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Absorb", IE_Pressed, this, &AMDCharacter::CollectPickups);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &AMDCharacter::Sprinting);
	InputComponent->BindAction("Sprint", IE_Released, this, &AMDCharacter::NotSprinting);
	InputComponent->BindAction("SwimUp", IE_Pressed, this, &AMDCharacter::SwimUp);
	InputComponent->BindAction("SwimDown", IE_Pressed, this, &AMDCharacter::SwimDown);
	InputComponent->BindAction("Attack", IE_Pressed, this, &AMDCharacter::Attack);

	InputComponent->BindAxis("MoveForward", this, &AMDCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMDCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AMDCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AMDCharacter::LookUpAtRate);


	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AMDCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AMDCharacter::TouchStopped);
}


void AMDCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AMDCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AMDCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMDCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMDCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMDCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

/** Character sprinting increase the speed**/
void AMDCharacter::Sprinting() {

	//TODO: Check if players ability affecting speed is active and change based on ability
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed*SpeedFactor;
}

/** Character is swimming up go up**/
void AMDCharacter::SwimUp() {

}

/**Character is swimming down go down**/
void AMDCharacter::SwimDown() {

}

void AMDCharacter::Attack() {

}

/** Character no longer sprinting decrease the speed**/
void AMDCharacter::NotSprinting() {
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
}
void AMDCharacter::CollectPickups() {
	
	//Get all overlapping Actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);
	if (CollectedActors.Num() > 3) {

		//Keep track of collected pickup
		float CollectedEnergy = 0;
		float CollectedHealth = 0;
		//For each Actor we collected
		for (int32 i = 0; i < CollectedActors.Num(); ++i) {

			//Cast the actor to APickup
			APickup* const TestPickup = Cast<APickup>(CollectedActors[i]);

			//If the cast is successful and the pickup is valid and active
			if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive()) {

				//call the pickups WasCollected function
				TestPickup->WasCollected();

				//Check if pickup is energy or Health by casting
				AEnergyPickup* const TestEnergy = Cast<AEnergyPickup>(TestPickup);
				AHealthPickup* const TestHealth = Cast<AHealthPickup>(TestPickup);
					
				//If pickup is energy add the collected energy is value of energy power
				if (TestEnergy) {
					CollectedEnergy += TestEnergy->GetEnergyPower();
					
					//If players power is not full
					if (CurrentPower != MaxPower) {
						//Deactivate the pickup
						TestPickup->SetActive(false);
						TestPickup->Destroy();

						//If the collected energy/health changed add it to character
						if (CollectedEnergy > 0) {
							UpdatePower(CollectedEnergy);
						}
					}
				}

				//If Pickup is health add the collected health is value of health value
				if (TestHealth){
					CollectedHealth += TestHealth->GetHealthValue();
					if (CurrentHealth != MaxHealth) {
						//Deactivate the pickup
						TestPickup->SetActive(false);
						TestPickup->Destroy();
						
						
						//If the collected energy/health changed add it to character
						if (CollectedHealth > 0) {
							UpdateHealth(CollectedHealth);
						}
					}
				}	
			}
		}
	}
}	

/**Accessor for Initial Power**/
float AMDCharacter::GetInitialPower() {
	return InitialPower;
}

/**Accessor for current power**/
float AMDCharacter::GetCurrentPower() {
	return CurrentPower;
}

/**Returns the max power/energy of player**/
float AMDCharacter::GetMaxPower() {
	return MaxPower;
}
/**Updates character power (Can be postive or negative)**/
void AMDCharacter::UpdatePower(float change) {

	CurrentPower += change;
	if (CurrentPower > MaxPower) {
		CurrentPower = MaxPower;
	}
	PowerChangeEffect();
	UE_LOG(LogClass, Log, TEXT("Current Energy: %f"), CurrentPower);

}

/**Accessor for Initial Health**/
float AMDCharacter::GetInitalHealth() {
	return InitialHealth;
}

/**Accessor for current Health**/
float AMDCharacter::GetCurrentHealth() {
	return CurrentHealth;
}

/** Returns the max health of player**/
float AMDCharacter::GetMaxHealth() {
	return MaxHealth;
}

/**Updates character Health (Can be postive or negative)**/
void AMDCharacter::UpdateHealth(float change) {

	CurrentHealth += change;
	if (CurrentHealth > MaxHealth) {
		CurrentHealth = MaxHealth;
	}
	UE_LOG(LogClass, Log, TEXT("Current Health: %f"), CurrentHealth);

}