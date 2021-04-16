#pragma once
#include "FrameVariable.h"
#include "TimeData.generated.h"

USTRUCT(BlueprintType)
struct FTimeData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Velocity;
	UPROPERTY(EditInstanceOnly)
	FVector Location;
	UPROPERTY(EditInstanceOnly)
	FQuat Rotation;
	float DeltaTime;
	FVector AngularVelocity;
	// TODO getters and setters


	
	void SaveFLoat(FName VariableName, float Value);

	void SaveVector(FName VariableName, FVector Value);

	void SaveQuat(FName VariableName, FQuat Value);

	FVector GetSavedVector(FName VariableName);

	FQuat GetSavedQuat(FName VariableName);

	float GetSavedFloat(FName VariableName);

private:
	TMap<FName, FrameVariable> SavedVariables;
};