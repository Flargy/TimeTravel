// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimeSystem/TimeSystem.h"
#include "TimeTravelGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TIMETRAVEL_API ATimeTravelGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATimeTravelGameModeBase();
	void Tick(float DeltaTime) override;
	TimeSystem TimeSystemInstance;
};
