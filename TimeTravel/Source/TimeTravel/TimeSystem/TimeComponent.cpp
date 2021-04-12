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


}

void UTimeComponent::BeginReverse()
{
	UE_LOG(LogTemp, Log, TEXT("Starting reverse"));
	OwningActor->SetActorTickEnabled(false);

}

void UTimeComponent::EndReverse()
{
	OwningActor->SetActorTickEnabled(true);
	
}

void UTimeComponent::ReverseTick(float DeltaTime)
{

}

