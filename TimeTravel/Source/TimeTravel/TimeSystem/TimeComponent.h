#pragma once
#include "Components/ActorComponent.h"
#include "TimeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

	void BeginReverse();
	void EndReverse();
	void ReverseTick(float DeltaTime);

private:

	AActor* OwningActor;
	UPROPERTY(VisibleAnywhere)
	TArray<UPrimitiveComponent*> PhysicsComponents;
};