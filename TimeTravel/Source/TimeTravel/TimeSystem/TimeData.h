#pragma once
#include "FrameVariable.h"

struct FTimeData
{
	float DeltaTime;

	// C++ hard coded variables just to demo the functionality
	FVector Velocity;
	FVector Location;
	FQuat Rotation;
	FVector AngularVelocity;

	// Savable data types save functions
	void SaveVector(FName VariableName, FVector Value);
	void SaveQuat(FName VariableName, FQuat Value);
	void SaveFLoat(FName VariableName, float Value);
	void SaveInt(FName VariableName, int Value);
	void SaveBool(FName VariableName, bool Value);

	// Savable data types load functions
	FVector GetSavedVector(FName VariableName, FVector DefaultReturnValue);
	FQuat GetSavedQuat(FName VariableName, FQuat DefaultReturnValue);
	float GetSavedFloat(FName VariableName, float DefaultReturnValue);
	int GetSavedInt(FName VariableName, int DefaultReturnValue);
	bool GetSavedBool(FName VariableName, bool DefaultReturnValue);

private:
	// Map storing saved data
	TMap<FName, FrameVariable> SavedVariables;
};