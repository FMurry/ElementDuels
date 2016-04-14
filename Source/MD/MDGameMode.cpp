// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MD.h"
#include "MDGameMode.h"
#include "MDCharacter.h"
#include "Blueprint/UserWidget.h"


AMDGameMode::AMDGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> ElectricPlayer(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacterElectric"));
	static ConstructorHelpers::FClassFinder<APawn> FirePlayer(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacterFire"));

	DefaultPawnClass = NULL;
	
}

void AMDGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMDGameMode::BeginPlay() {
	Super::BeginPlay();

	if (HudWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);
		if (CurrentWidget != nullptr) {
			CurrentWidget->AddToViewport();
		}
	}

}

