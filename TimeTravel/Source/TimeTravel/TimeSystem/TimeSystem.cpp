#include "TimeSystem.h"
#include "TimeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsSettings.h"

void TimeSystem::RegisterTimeComponent(UTimeComponent* compToAdd)
{
	UE_LOG(LogTemp, Log, TEXT("Adding thingy"));
	ReverseComponents.Add(compToAdd);
}

void TimeSystem::UnregisterTimeComponent(UTimeComponent* compToRemove)
{
	ReverseComponents.Remove(compToRemove);
}

void TimeSystem::BeginRewind()
{
	IsRewinding = true;

	if (TimeRewinded < 0.f)
		TimeRewinded = 0.f;

	for (UTimeComponent* comp : ReverseComponents)
	{
		comp->BeginReverse();
	}
}

void TimeSystem::EndRewind()
{
	if (!IsRewinding)
		return;
	IsRewinding = false;

	for (UTimeComponent* comp : ReverseComponents)
	{
		comp->EndReverse();
	}
}

void TimeSystem::Tick(float DeltaTime)
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

float TimeSystem::GetSecondsToRewind()
{
	return SecondsToRewind;
}
