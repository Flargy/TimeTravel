#pragma once

class UTimeComponent;
class ATimeTravelGameModeBase;

class FTimeSystem
{

public:
	void RegisterTimeComponent(UTimeComponent* compToAdd);
	void UnregisterTimeComponent(UTimeComponent* compToRemove);

	void BeginRewind();
	void EndRewind();
	void Tick(float DeltaTime);
	float GetSecondsToRewind();

private:
	TArray<UTimeComponent*> ReverseComponents;
	bool IsRewinding = false;

	// The maximum amount of seconds that can be reversed at a given time
	float SecondsToRewind = 5.0f;
	
	float TimeRewinded = 0.f;
};