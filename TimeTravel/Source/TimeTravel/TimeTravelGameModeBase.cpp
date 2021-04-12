// Copyright Epic Games, Inc. All Rights Reserved.


#include "TimeTravelGameModeBase.h"

ATimeTravelGameModeBase::ATimeTravelGameModeBase() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATimeTravelGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeSystemInstance.Tick(DeltaTime);
}
