#pragma once
//#include "TimeSystem.generated.h"

class UTimeComponent;
class ATimeTravelGameModeBase;

class TimeSystem
{

public:
	void RegisterTimeComponent(UTimeComponent* compToAdd);
	void UnregisterTimeComponent(UTimeComponent* compToRemove);

	void BeginRewind();
	void EndRewind();
	void Tick(float DeltaTime);

private:
	TArray<UTimeComponent*> ReverseComponents;
	bool IsRewinding = false;
};