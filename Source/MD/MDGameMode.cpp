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

	int32 RandomCharacter = FMath::FRandRange(0, 100);
	UE_LOG(LogClass, Log, TEXT("Random number is %d"),RandomCharacter);

	if (RandomCharacter%2 == 0) {
		if (ElectricPlayer.Class != NULL) {
			DefaultPawnClass = ElectricPlayer.Class;
		}
	}
	else if (RandomCharacter%2 == 1) {
		if (FirePlayer.Class != NULL) {
			DefaultPawnClass = FirePlayer.Class;
		}
	}
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

