#include "TimeData.h"


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

void FTimeData::SaveFLoat(FName VariableName, float Value)
{
	FrameVariable NewVariable;
	NewVariable.SavedFloat = Value;
	SavedVariables.Add(VariableName, NewVariable);
}

void FTimeData::SaveInt(FName VariableName, int Value)
{
	FrameVariable NewVariable;
	NewVariable.SavedInt = Value;
	SavedVariables.Add(VariableName, NewVariable);
}

void FTimeData::SaveBool(FName VariableName, bool Value)
{
	FrameVariable NewVariable;
	NewVariable.SavedBool = Value;
	SavedVariables.Add(VariableName, NewVariable);
}

FVector FTimeData::GetSavedVector(FName VariableName, FVector DefaultReturnValue)
{
	if (!SavedVariables.Contains(VariableName))
	{
		UE_LOG(LogTemp, Warning, TEXT("The given Vector variable was not found: %s"), *VariableName.ToString());
		return DefaultReturnValue;
	}

	return SavedVariables[VariableName].SavedVector;
}

FQuat FTimeData::GetSavedQuat(FName VariableName, FQuat DefaultReturnValue)
{
	if (!SavedVariables.Contains(VariableName))
	{
		UE_LOG(LogTemp, Warning, TEXT("The given Quat variable was not found: %s"), *VariableName.ToString());
		return DefaultReturnValue;
	}
	return SavedVariables[VariableName].SavedQuat;
}

float FTimeData::GetSavedFloat(FName VariableName, float DefaultReturnValue)
{
	if (!SavedVariables.Contains(VariableName))
	{
		UE_LOG(LogTemp, Warning, TEXT("The given float variable was not found: %s"), *VariableName.ToString());
		return DefaultReturnValue;
	}
	return SavedVariables[VariableName].SavedFloat;
}

int FTimeData::GetSavedInt(FName VariableName, int DefaultReturnValue)
{
	if (!SavedVariables.Contains(VariableName))
	{
		UE_LOG(LogTemp, Warning, TEXT("The given int variable was not found: %s"), *VariableName.ToString());
		return DefaultReturnValue;
	}
	return SavedVariables[VariableName].SavedInt;
}

bool FTimeData::GetSavedBool(FName VariableName, bool DefaultReturnValue)
{
	if (!SavedVariables.Contains(VariableName))
	{
		UE_LOG(LogTemp, Warning, TEXT("The given bool variable was not found: %s"), *VariableName.ToString());
		return DefaultReturnValue;
	}
	return SavedVariables[VariableName].SavedBool;
}
