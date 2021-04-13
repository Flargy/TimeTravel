#include "TimeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimeTravel/TimeTravelGameModeBase.h"


void UTimeComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningActor = GetOwner();
	auto GameMode = Cast<ATimeTravelGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
		GameMode->TimeSystemInstance.RegisterTimeComponent(this);

	PhysicsComponents.Empty();

	TArray<UActorComponent*> ActorComponents = OwningActor->K2_GetComponentsByClass(UPrimitiveComponent::StaticClass());

	for (UActorComponent* Component : ActorComponents)
	{
		UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Component);
		PhysicsComponents.Add(Primitive);
	}


}


void UTimeComponent::BeginReverse()
{
	OwningActor->SetActorTickEnabled(false);
	for (UPrimitiveComponent* PrimitiveComp : PhysicsComponents)
	{
		PrimitiveComp->SetSimulatePhysics(false);
	}
}

void UTimeComponent::EndReverse()
{
	OwningActor->SetActorTickEnabled(true);
	for (UPrimitiveComponent* PrimitiveComp : PhysicsComponents)
	{
		PrimitiveComp->SetSimulatePhysics(true);
	}
}

void UTimeComponent::ReverseTick(float DeltaTime)
{

}

