// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SeigeGameMode.h"
#include "SeigeHUD.h"
#include "SeigeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASeigeGameMode::ASeigeGameMode()
	: Super()
{
	// use our custom HUD class
	HUDClass = ASeigeHUD::StaticClass();
}
