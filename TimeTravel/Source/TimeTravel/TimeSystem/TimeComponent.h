#pragma once
#include "Components/ActorComponent.h"
#include "TimeData.h"
#include "TimeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTimeComponent();
	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BeginReverse();
	void EndReverse();
	void ReverseTick(float DeltaTime);

private:
	void PerformCleanup();

	AActor* OwningActor;

	UPROPERTY(VisibleAnywhere)
	UPrimitiveComponent* PhysicsComponent;

	UPROPERTY(EditAnywhere)
	float SecondsToRewind;

	float SecondSinceLastCleanup = 0.f;

	int FramesSincelastCleanup = 0;

	int CurrentRewindFrame = 0;
	float TimeInCurrentRewindFrame = 0.0f;


	TArray<FTimeData> SavedData;
};