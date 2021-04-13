#pragma once
#include "TimeData.generated.h"

USTRUCT(BlueprintType)
struct FTimeData
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly)
	FVector Velocity;
	UPROPERTY(EditInstanceOnly)
	FVector Location;
	UPROPERTY(EditInstanceOnly)
	FQuat Rotation;
	float DeltaTime;
};