#include "TimeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimeTravel/TimeTravelGameModeBase.h"
#include "Math/UnrealMathVectorCommon.h"


UTimeComponent::UTimeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

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


void UTimeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	FTimeData FrameData;
	FTransform OwnerTransform = OwningActor->GetTransform();
	FrameData.Location = OwnerTransform.GetLocation();
	FrameData.Rotation = OwnerTransform.GetRotation();
	FrameData.Velocity = OwningActor->GetVelocity();
	FrameData.DeltaTime = DeltaTime;

	SavedData.Add(FrameData);

	FramesSincelastCleanup++;
	SecondSinceLastCleanup += DeltaTime;

	if (SecondSinceLastCleanup > SecondsToRewind)
	{
		PerformCleanup();
	}

}

void UTimeComponent::BeginReverse()
{
	OwningActor->SetActorTickEnabled(false);
	SetComponentTickEnabled(false);
	for (UPrimitiveComponent* PrimitiveComp : PhysicsComponents)
	{
		PrimitiveComp->SetSimulatePhysics(false);
	}

	CurrentRewindFrame = SavedData.Num() - 1;
	TimeInCurrentRewindFrame = 0;
}

void UTimeComponent::EndReverse()
{
	OwningActor->SetActorTickEnabled(true);
	SetComponentTickEnabled(true);
	for (UPrimitiveComponent* PrimitiveComp : PhysicsComponents)
	{
		PrimitiveComp->SetSimulatePhysics(true);
	}


	SavedData.RemoveAt(CurrentRewindFrame, SavedData.Num() - CurrentRewindFrame);

}

void UTimeComponent::ReverseTick(float DeltaTime)
{
	TimeInCurrentRewindFrame -= DeltaTime;
	while (TimeInCurrentRewindFrame < 0.0f && CurrentRewindFrame > 0)
	{
		TimeInCurrentRewindFrame += SavedData[CurrentRewindFrame].DeltaTime;
		FramesSincelastCleanup--;
		SecondSinceLastCleanup -= SavedData[CurrentRewindFrame].DeltaTime;
		CurrentRewindFrame--;

	}

	TimeInCurrentRewindFrame = TimeInCurrentRewindFrame < 0.f ? 0.f : TimeInCurrentRewindFrame;

	float LerpT = TimeInCurrentRewindFrame / SavedData[CurrentRewindFrame + 1].DeltaTime;



	FTimeData CurrentFrameData = SavedData[CurrentRewindFrame];
	FTimeData PreviousFrameData = SavedData[CurrentRewindFrame + 1];

	FVector NewPosition = FMath::Lerp(CurrentFrameData.Location, PreviousFrameData.Location, LerpT);
	OwningActor->SetActorLocation(NewPosition);

	// TODO lerp rotation and velocity between SavedData[CurrentRewindFrame] and SavedData[CurrentRewindFrame + 1]

	UE_LOG(LogTemp, Log, TEXT("X: %f Y: %f Z: %f"), NewPosition.X, NewPosition.Y, NewPosition.Z);
}

void UTimeComponent::PerformCleanup()
{
	SavedData.RemoveAt(0, SavedData.Num() - FramesSincelastCleanup, true);
	FramesSincelastCleanup = 0;
	SecondSinceLastCleanup = 0.f;
}

