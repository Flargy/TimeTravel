#include "TimeData.h"

void FTimeData::SaveFLoat(FName VariableName, float Value)
{
	FrameVariable NewVariable;
	NewVariable.SavedFloat = Value;
	SavedVariables.Add(VariableName, NewVariable);
}

void FTimeData::SaveVector(FName VariableName, FVector Value)
{
	FrameVariable NewVariable;
	NewVariable.SavedVector = Value;
	SavedVariables.Add(VariableName, NewVariable);
}

void FTimeData::SaveQuat(FName VariableName, FQuat Value)
{
	FrameVariable NewVariable;
	NewVariable.SavedQuat = Value;
	SavedVariables.Add(VariableName, NewVariable);
}


FVector FTimeData::GetSavedVector(FName VariableName)
{
	return SavedVariables[VariableName].SavedVector;
}

FQuat FTimeData::GetSavedQuat(FName VariableName)
{
	return SavedVariables[VariableName].SavedQuat;
}

float FTimeData::GetSavedFloat(FName VariableName)
{
	return SavedVariables[VariableName].SavedFloat;
}
