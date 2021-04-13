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

	
	
	
	for (UTimeComponent* comp : ReverseComponents)
	{
		comp->BeginReverse();
	}
}

void TimeSystem::EndRewind()
{
	IsRewinding = false;

	


	for (UTimeComponent* comp : ReverseComponents)
	{
		comp->EndReverse();
	}
}

void TimeSystem::Tick(float DeltaTime)
{
	if (!IsRewinding)
		return;

	for (UTimeComponent* comp : ReverseComponents)
	{
		comp->ReverseTick(DeltaTime);
	}
}
