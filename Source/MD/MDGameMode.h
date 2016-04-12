// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "MDGameMode.generated.h"

UCLASS(minimalapi)
class AMDGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMDGameMode();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
protected:
	/** THe Widget Class to use for our HUD screen**/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category =  "Game", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HudWidgetClass;

	/**The instance of The HUD**/
	UPROPERTY()
	class UUserWidget* CurrentWidget;



};



