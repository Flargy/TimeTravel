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
	{
		GameMode->TimeSystemInstance.RegisterTimeComponent(this);
		SecondsToRewind = GameMode->TimeSystemInstance.GetSecondsToRewind();
	}

	PhysicsComponent = Cast<UPrimitiveComponent>(OwningActor->GetRootComponent());
	
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
	FrameData.AngularVelocity = PhysicsComponent->GetPhysicsAngularVelocityInRadians();

	TestSave.Broadcast(FrameData);

	SavedData.Add(FrameData);
	SaveData.Broadcast();

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
	PhysicsComponent->SetSimulatePhysics(false);
	
	CurrentRewindFrame = SavedData.Num() - 1;
	TimeInCurrentRewindFrame = 0;
}

void UTimeComponent::EndReverse()
{
	OwningActor->SetActorTickEnabled(true);
	SetComponentTickEnabled(true);
	PhysicsComponent->SetSimulatePhysics(true);
	
	FTimeData CurrentFrameData = SavedData[CurrentRewindFrame];

	PhysicsComponent->SetPhysicsLinearVelocity(CurrentFrameData.Velocity);
	PhysicsComponent->SetPhysicsAngularVelocityInRadians(CurrentFrameData.AngularVelocity);
	OwningActor->SetActorLocation(CurrentFrameData.Location);
	OwningActor->SetActorRotation(CurrentFrameData.Rotation);
	

	SavedData.RemoveAt(CurrentRewindFrame, SavedData.Num() - CurrentRewindFrame);

}

void UTimeComponent::ReverseTick(float DeltaTime)
{
	TimeInCurrentRewindFrame -= DeltaTime;
	while (TimeInCurrentRewindFrame < 0.0f && CurrentRewindFrame > 0)
	{
		SecondSinceLastCleanup -= SavedData[CurrentRewindFrame].DeltaTime;
		FramesSincelastCleanup--;
		TimeInCurrentRewindFrame += SavedData[CurrentRewindFrame].DeltaTime;
		CurrentRewindFrame--;
	}

	TimeInCurrentRewindFrame = TimeInCurrentRewindFrame < 0.f ? 0.f : TimeInCurrentRewindFrame;

	LerpT = TimeInCurrentRewindFrame / SavedData[CurrentRewindFrame + 1].DeltaTime;


	FTimeData CurrentFrameData = SavedData[CurrentRewindFrame];
	FTimeData PreviousFrameData = SavedData[CurrentRewindFrame + 1];

	FVector NewPosition = FMath::Lerp(CurrentFrameData.Location, PreviousFrameData.Location, LerpT);
	OwningActor->SetActorLocation(NewPosition);

	FQuat NewQuat = FQuat::Slerp(CurrentFrameData.Rotation, PreviousFrameData.Rotation, LerpT);
	OwningActor->SetActorRotation(NewQuat);

	LoadData.Broadcast(CurrentRewindFrame, LerpT);
	
	//UE_LOG(LogTemp, Log, TEXT("X: %f Y: %f Z: %f"), NewPosition.X, NewPosition.Y, NewPosition.Z);
}


void UTimeComponent::SaveFloat(FName VariableName, float Value)
{
	SavedData[SavedData.Num() - 1].SaveFLoat(VariableName, Value);
}

void UTimeComponent::SaveVector(FName VariableName, FVector Value)
{
	SavedData[SavedData.Num() - 1].SaveVector(VariableName, Value);
}

FVector UTimeComponent::LoadSavedVector(FName VariableName, bool Interpolated)
{
	FVector ToReturn = SavedData[CurrentRewindFrame].GetSavedVector(VariableName);

	if (Interpolated == true)
	{
		ToReturn = FMath::Lerp(ToReturn, SavedData[CurrentRewindFrame + 1].GetSavedVector(VariableName), LerpT);
	}

	return ToReturn;
}

void UTimeComponent::PerformCleanup()
{
	int FramesToRemove = SavedData.Num() - FramesSincelastCleanup;
	if (FramesToRemove > 0)
	{
		CleanupArray.Broadcast(FramesToRemove - 1);

		UE_LOG(LogTemp, Log, TEXT("Array size before removal:  %d"), SavedData.Num());
		SavedData.RemoveAt(0, FramesToRemove, true);
		UE_LOG(LogTemp, Log, TEXT("Array size after removal:  %d"), SavedData.Num());
	}
	FramesSincelastCleanup = 0;
	SecondSinceLastCleanup = 0.f;
}

