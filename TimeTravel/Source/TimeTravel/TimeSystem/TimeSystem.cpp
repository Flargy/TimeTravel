#include "TimeSystem.h"
#include "TimeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsSettings.h"


/**
 * Function for registering a TimeComponent for reversal of time
 */
void FTimeSystem::RegisterTimeComponent(UTimeComponent* compToAdd)
{
	ReverseComponents.Add(compToAdd);
}

/**
 * Function for unregistering a TimeComponent from time reversal
 */
void FTimeSystem::UnregisterTimeComponent(UTimeComponent* compToRemove)
{
	ReverseComponents.Remove(compToRemove);
}

/**
 * Starts the rewind and activates BeginReverse on each registered TimeComponent
 */
void FTimeSystem::BeginRewind()
{
	IsRewinding = true;

	if (TimeRewinded < 0.f)
		TimeRewinded = 0.f;

	for (UTimeComponent* comp : ReverseComponents)
	{
		comp->BeginReverse();
	}
}

/**
 * Ends the rewind and activates EndReverse on each registered TimeComponent
 */
void FTimeSystem::EndRewind()
{
	if (!IsRewinding)
		return;
	IsRewinding = false;

	for (UTimeComponent* comp : ReverseComponents)
	{
		comp->EndReverse();
	}
}

/**
 * Increments TimeRewinded when rewinding and Decrements it when time isn't being rewinded.
 * Calls ReverseTick on each registered TimeComponent each frame when rewinding
 */
void FTimeSystem::Tick(float DeltaTime)
{
	if (!IsRewinding)
	{
		TimeRewinded -= DeltaTime;
		return;
	}

	TimeRewinded += DeltaTime;
	if (TimeRewinded >= SecondsToRewind)
	{
		EndRewind();
		return;
	}

	for (UTimeComponent* comp : ReverseComponents)
	{
		comp->ReverseTick(DeltaTime);
	}
}

/**
 * Returns the maximum number of seconds that can be rewinded at a time
 */
float FTimeSystem::GetSecondsToRewind()
{
	return SecondsToRewind;
}
